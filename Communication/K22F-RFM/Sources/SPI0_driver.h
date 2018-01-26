#include "fsl_device_registers.h"
#include "stdio.h"
#include "stdlib.h"

#define SPI_PUSHR_PCS0_ON 0x10000 // used by SPI0_TX

void SPI0_Init(int frame_size){
	// taken from reference manual and https://community.nxp.com/thread/372146#comment-562567 (For FRDM-K64F)

	// initialize the SPI bus
	// CS -> D0
	// CLK -> D1
	// MOSI -> D2
	// MISO -> D3

	// clock enables for GPIO and SPI
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK; // enable GPIO D
	SIM_SCGC6 |= SIM_SCGC6_SPI0_MASK; // enable SPI0

	// mux pins for SPI and UART
	PORTD_PCR(0) |= PORT_PCR_MUX(2); // Chip select
	PORTD_PCR(1) |= PORT_PCR_MUX(2); // Clock
	PORTD_PCR(2) |= PORT_PCR_MUX(2); // MOSI
	PORTD_PCR(3) |= PORT_PCR_MUX(2); // MISO

	// reset all flags by writing ones
	SPI0_SR = SPI_SR_TCF_MASK | SPI_SR_EOQF_MASK | SPI_SR_TFUF_MASK | SPI_SR_TFFF_MASK | SPI_SR_RFOF_MASK | SPI_SR_RFDF_MASK;

	// reset registers
	SPI0_TCR = 0;
	SPI0_RSER = 0;
	SPI0_PUSHR = 0; // no transmission since SPI0 is halted
	SPI0_CTAR0 = 0;

	// configure SPI0
	SPI0_CTAR0 |= SPI_CTAR_FMSZ(frame_size-1) | SPI_CTAR_CPOL_MASK | SPI_CTAR_BR(0x9) | SPI_CTAR_CPHA_MASK; // | SPI_CTAR_ASC(0xFF); // 16 bit frames, active low clock, ??? baud rate clock, BR factor 2
	SPI0_MCR |= SPI_MCR_MSTR_MASK | SPI_MCR_PCSIS_MASK; // master, CS active low
	SPI0_MCR &= (~SPI_MCR_DIS_RXF_MASK) | (~SPI_MCR_DIS_TXF_MASK); // enable rx and tx FIFOs
	SPI0_MCR &= (~SPI_MCR_MDIS_MASK) & (~SPI_MCR_HALT_MASK); // enable module clock and start transfers
}

// configure SPI to start transfer
void SPI0_Prep(){

	// halt SPI module
	SPI0_MCR |= SPI_MCR_HALT_MASK;

	// flush FIFOs
	SPI0_MCR |= SPI_MCR_CLR_RXF_MASK | SPI_MCR_CLR_TXF_MASK;

	// ensure no freezing for debugging
	SPI0_MCR &= ~SPI_MCR_FRZ_MASK;

	// clear the status bits by writing ones
	SPI0_SR |= (SPI_SR_TCF_MASK | SPI_SR_EOQF_MASK | SPI_SR_TFUF_MASK | SPI_SR_TFFF_MASK | SPI_SR_RFOF_MASK | SPI_SR_RFDF_MASK);

	// enable transfers and reset counter
	SPI0_TCR &= ~SPI_TCR_SPI_TCNT_MASK; // reset transfer counter to 0
	SPI0_MCR &= ~SPI_MCR_HALT_MASK; // enable transfers
}

uint8_t SPI0_RX(){
	uint8_t temp;

	// wait for receive fifo drain flag (RFDF) to go to 1
	while(!(SPI0_SR & SPI_SR_RFDF_MASK));

	// save the rx data
	temp = SPI0_POPR;

	// clear RFDF flag
	SPI0_SR |= SPI_SR_RFDF_MASK;

	return temp;
}

void SPI0_TX(uint8_t tx_data){
	SPI0_PUSHR =  (SPI_PUSHR_PCS0_ON | tx_data); // may need to be changed based on chip, CS0, SPI_PUSHR_CONT_MASK

	// wait for transmission to complete flag to go to 1 (TCF)
	while(!(SPI0_SR & SPI_SR_TCF_MASK));

	// clear status bits, this may not need to be done
	SPI0_SR |= SPI_SR_TFFF_MASK;
}
