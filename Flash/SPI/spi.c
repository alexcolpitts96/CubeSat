/*
 * spi.c
 *
 *  Created on: Mar 9, 2018
 *      Author: quinn
 */

#include "fsl_device_registers.h"

void SPI1_Init(int frame_size){
	// taken from reference manual and https://community.nxp.com/thread/372146#comment-562567 (For FRDM-K64F)

	// initialize the SPI bus
	// CS -> D4
	// CLK -> D5
	// MOSI -> D6
	// MISO -> D7

	// clock enables for GPIO and SPI
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK; // enable GPIO D
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK; // enable GPIO E
	SIM_SCGC6 |= SIM_SCGC6_SPI1_MASK; // enable SPI1

	// mux pins for SPI
	PORTD_PCR4 |= PORT_PCR_MUX(7); // CS0
	PORTE_PCR0 |= PORT_PCR_MUX(2); // CS1
	PORTD_PCR6|= PORT_PCR_MUX(7); // MOSI
	PORTD_PCR7 |= PORT_PCR_MUX(7); // MISO
	PORTD_PCR5 |= PORT_PCR_MUX(7); // SCK

	// reset all flags by writing ones
	SPI1_SR = SPI_SR_TCF_MASK | SPI_SR_EOQF_MASK | SPI_SR_TFUF_MASK | SPI_SR_TFFF_MASK | SPI_SR_RFOF_MASK | SPI_SR_RFDF_MASK;

	// reset registers
	SPI1_TCR = 0;
	SPI1_RSER = 0;
	SPI1_PUSHR = 0; // no transmission since SPI1 is halted
	SPI1_CTAR0 = 0;

	// configure SPI1
	SPI1_CTAR0 |= SPI_CTAR_FMSZ(frame_size-1) | SPI_CTAR_BR(0x9); // CPHA = 0, CPOL = 0, BR can be changed with parameter in brackets
	SPI1_MCR |= SPI_MCR_MSTR_MASK | SPI_MCR_PCSIS_MASK; // master, CS active low
	SPI1_MCR &= (~SPI_MCR_DIS_RXF_MASK) | (~SPI_MCR_DIS_TXF_MASK); // enable rx and tx FIFOs
	SPI1_MCR &= (~SPI_MCR_MDIS_MASK) & (~SPI_MCR_HALT_MASK); // enable module clock and start transfers
}

// configure SPI to start transfer
void SPI1_txPrep(){

	// halt SPI module
	SPI1_MCR |= SPI_MCR_HALT_MASK;

	// flush FIFOs
	SPI1_MCR |= SPI_MCR_CLR_RXF_MASK | SPI_MCR_CLR_TXF_MASK;

	// ensure no freezing for debugging
	SPI1_MCR &= ~SPI_MCR_FRZ_MASK;

	// clear the status bits by writing ones
	SPI1_SR |= (SPI_SR_TCF_MASK | SPI_SR_EOQF_MASK | SPI_SR_TFUF_MASK | SPI_SR_TFFF_MASK | SPI_SR_RFOF_MASK | SPI_SR_RFDF_MASK);

	// enable transfers and reset counter
	SPI1_TCR &= ~SPI_TCR_SPI_TCNT_MASK; // reset transfer counter to 0
	SPI1_MCR &= ~SPI_MCR_HALT_MASK; // enable transfers
}

uint16_t SPI1_RX(){
	uint16_t temp;

	// wait for receive fifo drain flag (RFDF) to go to 1
	while(!(SPI1_SR & SPI_SR_RFDF_MASK));

	// save the rx data
	temp = SPI1_POPR;

	// clear RFDF flag
	SPI1_SR |= SPI_SR_RFDF_MASK;

	return temp;
}

void SPI1_TX(uint16_t tx_data){
	SPI1_txPrep();
	// wait for tx fifo to not be full
	while(!(SPI1_SR & SPI_SR_TFFF_MASK)); // clock timing issue not coming from wait here

	SPI1_PUSHR = SPI_PUSHR_PCS(1) | tx_data; // send to chip with CS0

	// wait for transmission to complete flag to go to 1 (TCF)
	while(!(SPI1_SR & SPI_SR_TCF_MASK));

	// clear status bits, this may not need to be done
	SPI1_SR |= SPI_SR_TFFF_MASK;
}

uint8_t SPI1_read(uint8_t regaddr){
	SPI1_TX(regaddr << 8);
	return (SPI1_RX() & 0x00FF);
}
