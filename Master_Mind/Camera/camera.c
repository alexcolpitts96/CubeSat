/*
 * camera.c
 *
 *  Created on: Feb 2, 2018
 *      Author: quinn
 *
 *      SPI drivers adapted from Alex Colpitts' SPI0 drivers.
 *
 *      All other functions are specific to handling the camera module.
 */

#include "fsl_device_registers.h"
#include "camera.h"
#include "../I2C/i2c.h"
#include "../Comms/Comms.h"
#include "math.h"

void SPI1_Init(int frame_size) {
	// taken from reference manual and https://community.nxp.com/thread/372146#comment-562567 (For FRDM-K64F)

	// initialize the SPI bus
	// CS -> D4
	// CLK -> D5
	// MOSI -> D6
	// MISO -> D7

	// clock enables for GPIO and SPI
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK; // enable GPIO D
	SIM_SCGC6 |= SIM_SCGC6_SPI1_MASK; // enable SPI1

	// mux pins for SPI
	PORTD_PCR4 |= PORT_PCR_MUX(7); // CS
	PORTD_PCR6 |= PORT_PCR_MUX(7); // MOSI
	PORTD_PCR7 |= PORT_PCR_MUX(7); // MISO
	PORTD_PCR5 |= PORT_PCR_MUX(7); // SCK

	// reset all flags by writing ones
	SPI1_SR = SPI_SR_TCF_MASK | SPI_SR_EOQF_MASK | SPI_SR_TFUF_MASK
			| SPI_SR_TFFF_MASK | SPI_SR_RFOF_MASK | SPI_SR_RFDF_MASK;

	// reset registers
	SPI1_TCR = 0;
	SPI1_RSER = 0;
	SPI1_PUSHR = 0; // no transmission since SPI1 is halted
	SPI1_CTAR0 = 0;

	// configure SPI1
	SPI1_CTAR0 |= SPI_CTAR_FMSZ(frame_size-1) | SPI_CTAR_BR(0xC); // CPHA = 0, CPOL = 0, BR can be changed with parameter in brackets
	SPI1_MCR |= SPI_MCR_MSTR_MASK | SPI_MCR_PCSIS_MASK; // master, CS active low
	SPI1_MCR &= (~SPI_MCR_DIS_RXF_MASK) | (~SPI_MCR_DIS_TXF_MASK); // enable rx and tx FIFOs
	SPI1_MCR &= (~SPI_MCR_MDIS_MASK) & (~SPI_MCR_HALT_MASK); // enable module clock and start transfers
}

// configure SPI to start transfer
void SPI1_txPrep() {

	// halt SPI module
	SPI1_MCR |= SPI_MCR_HALT_MASK;

	// flush FIFOs
	SPI1_MCR |= SPI_MCR_CLR_RXF_MASK | SPI_MCR_CLR_TXF_MASK;

	// ensure no freezing for debugging
	SPI1_MCR &= ~SPI_MCR_FRZ_MASK;

	// clear the status bits by writing ones
	SPI1_SR |= (SPI_SR_TCF_MASK | SPI_SR_EOQF_MASK | SPI_SR_TFUF_MASK
			| SPI_SR_TFFF_MASK | SPI_SR_RFOF_MASK | SPI_SR_RFDF_MASK);

	// enable transfers and reset counter
	SPI1_TCR &= ~SPI_TCR_SPI_TCNT_MASK; // reset transfer counter to 0
	SPI1_MCR &= ~SPI_MCR_HALT_MASK; // enable transfers
}

uint16_t SPI1_RX() {
	uint16_t temp;

	// wait for receive fifo drain flag (RFDF) to go to 1
	while (!(SPI1_SR & SPI_SR_RFDF_MASK))
		;

	// save the rx data
	temp = SPI1_POPR;

	// clear RFDF flag
	SPI1_SR |= SPI_SR_RFDF_MASK;

	return temp;
}

void SPI1_TX(uint16_t tx_data) {
	SPI1_txPrep();
	// wait for tx fifo to not be full
	while (!(SPI1_SR & SPI_SR_TFFF_MASK))
		; // clock timing issue not coming from wait here

	SPI1_PUSHR = SPI_PUSHR_PCS(1) | tx_data; // may need to be changed based on chip, CS0, SPI_PUSHR_CONT_MASK

	// wait for transmission to complete flag to go to 1 (TCF)
	while (!(SPI1_SR & SPI_SR_TCF_MASK))
		;

	// clear status bits, this may not need to be done
	SPI1_SR |= SPI_SR_TFFF_MASK;
}

uint8_t SPI1_read(uint8_t regaddr) {
	SPI1_TX(regaddr << 8);
	return (SPI1_RX() & 0x00FF);
}

void cam_cfg(struct ov2640_reg_cfg *vals) {
	while (((vals->reg) != 0xff) || ((vals->val) != 0xff)) { // both values aren't 0xff
		I2CWriteRegister(vals->reg, vals->val);
		vals++;
	}
	return;
}

// currently: JPEG format, 1600x1200
void camera_init() {
	I2CWriteRegister(0xff, 0x01);
	I2CWriteRegister(0x12, 0x80);
	cam_cfg(JPEG_INIT); // jpeg init
	cam_cfg(YUV_422); // yuv color select
	cam_cfg(JPEG); // jpeg select
	I2CWriteRegister(0xff, 0x01);
	I2CWriteRegister(0x15, 0x00);
	//cam_cfg(JPEG_SMALL); // 320x240 init
	cam_cfg(JPEG_LARGE); // 1600x1200 select
	return;
}

uint8_t cam_reg_read(uint8_t regaddr) {
	uint8_t tmp;
	SPI1_TX(regaddr << 8);
	tmp = SPI1_RX();
	Pause();
	return tmp;
}

uint32_t fifo_len() {
	uint32_t len1 = cam_reg_read(0x42); // bits 7-0
	uint32_t len2 = cam_reg_read(0x43); // bits 15-8
	uint32_t len3 = cam_reg_read(0x44); // bits 18-16
	return ((len3 << 16) | (len2 << 8) | len1) & 0x07fffff; // combine them, return length of fifo in
}

int capture_done() {
	return (cam_reg_read(0x41) & 0x08) >> 3; // return 1 for capture done, 0 otherwise
}

// read in the entire fifo and return a pointer to it in memory
void fifo_read(uint8_t **s) {
	// check fifo length for errors
	uint32_t len = fifo_len();
	if ((len == 0) || (len >= MAX_FIFO_LENGTH)) {
		return; // ERROR
	}

	int read_count = 0;
	int packet_number = (int) ceil((float) len / (float) PACKET_SIZE);

	for (int i = 0; i < packet_number; i++) {
		for (int j = 0; j < PACKET_SIZE; j++) {

			// if all bytes have been read return
			if (read_count >= len) {
				return;
			}

			// read into memory
			s[i][j] = cam_reg_read(0x3D);
			read_count++;
		}
	}

	return;
}

// sends capture command and returns pointer to start of image byte array (quite large)
void capture(uint8_t **image) {

	enable_fifo();
	flush_fifo(); // clear fifo flag/flush fifo
	start_capture();
	while (!(capture_done())); // check flag for capture complete
	fifo_read(image); // read entire fifo in

	flush_fifo(); // finished reading, so empty fifo
	return;
}
