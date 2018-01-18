/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// libraries
#include "fsl_device_registers.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "RFM69registers.h"

// definitions
#define RFM_WRITE 0x80
#define RFM_READ 0x00

#define SPI_CTAR_FMSZ_8BIT 0x38000000 // used by SPI0_Init
#define SPI_PUSHR_PCS0_ON 0x10000 // used by SPI0_Tx



void UART0_Init(){
	// initialize UART for PC display
	// no external connections required

	// clock enables for GPIO and UART
	SIM_SCGC4 |= SIM_SCGC4_UART0_MASK; // enable UART
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; // enable GPIO B

	// mux pins for GPIO and UART
	PORTB_PCR16 |= PORT_PCR_MUX(3); // RX, B16
	PORTB_PCR17 |= PORT_PCR_MUX(3); // TX, B17

	// calculate baud rate register value using ((21000*1000)/(baud_rate * 16))
	UART0_BDL |= 0x89;		// setting baud rate for UART0 to 9600

	// control registers for UART
	UART0_C1 |= 0; // no parity
	UART0_C2 |= UART_C2_RE_MASK | UART_C2_TE_MASK; // enable transmit and receive
	UART0_C3 |= UART_C3_PEIE_MASK | UART_C3_FEIE_MASK;
}

void UART1_Init(){
	// initialize UART for PC display
	// no external connections required

	// clock enables for GPIO and UART
	SIM_SCGC4 |= SIM_SCGC4_UART1_MASK; // enable UART 1
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; // enable GPIO C

	// mux pins for GPIO and UART
	PORTC_PCR3 |= PORT_PCR_MUX(3); // RX, C3
	PORTC_PCR4 |= PORT_PCR_MUX(3); // TX, C4

	// calculate baud rate register value using ((21000*1000)/(baud_rate * 16))
	UART1_BDL |= 0x89;		// setting baud rate for UART0 to 9600

	// control registers for UART
	UART1_C1 |= 0; // no parity
	UART1_C2 |= UART_C2_RE_MASK | UART_C2_TE_MASK; // enable transmit and receive
	UART1_C3 |= UART_C3_PEIE_MASK | UART_C3_FEIE_MASK;
}

// print a single character
void UART0_Putchar (char display_char){

	while(!(UART0_S1 & UART_S1_TC_MASK)){} // wait until the tx is ready for next char

	UART0_D = display_char; // when ready send next char
}

// print a single character
void UART1_Putchar (char display_char){

	while(!(UART1_S1 & UART_S1_TC_MASK)){} // wait until the tx is ready for next char

	UART1_D = display_char; // when ready send next char
}

// display string in terminal
void UART0_Putstring() {
	int i = 0;
	char welcome[80] = "Welcome to our program\0";

	while(welcome[i]){
		UART0_Putchar(welcome[i]);
		i++;
	}
}

// display string in terminal
void UART1_Putstring(uint8_t num) {
	int i = 0;
	char welcome[80] = "Test  \n\0";
	welcome[5] = num;

	while(welcome[i]){
		UART1_Putchar(welcome[i]);
		i++;
	}
}

void SPI0_Init(){
	// general concept taken from reference manual and https://community.nxp.com/thread/372146#comment-562567 (For FRDM-K64F)

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
	SPI0_CTAR0 |= SPI_CTAR_FMSZ_8BIT | SPI_CTAR_CPOL_MASK | SPI_CTAR_BR(0x9) | SPI_CTAR_CPHA_MASK; // 8 bit frames, active low clock, ??? baud rate clock, BR factor 2
	SPI0_MCR |= SPI_MCR_MSTR_MASK | SPI_MCR_PCSIS_MASK; // master, CS active low
	SPI0_MCR &= (~SPI_MCR_DIS_RXF_MASK) | (~SPI_MCR_DIS_TXF_MASK); // enable rx and tx FIFOs
	SPI0_MCR &= (~SPI_MCR_MDIS_MASK) & (~SPI_MCR_HALT_MASK); // enable module clock and start transfers

	// SPI_CTAR_FMSZ(8), potentially replace SPI_CTAR_FMSZ_8BIT macro
}

// write a byte to SPI0 MOSI
// NOTE: This does not check if RFM is ready
void SPI0_Tx(uint8_t tx_data){

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

	// send bits
	SPI0_PUSHR = (~SPI_PUSHR_CONT_MASK) & (SPI_PUSHR_PCS0_ON | tx_data); // may need to be changed based on chip, CS0

	// wait for transmission to complete flag to go to 1 (TCF)
	while(!(SPI0_SR & SPI_SR_TCF_MASK));

	// clear status bits, this may not need to be done
	SPI0_SR |= SPI_SR_TFFF_MASK;
}

// read from SPI0 MISO and return the value
// NOTE: This does not check if RFM is ready
uint8_t SPI0_Rx(){
	// halt SPI module
	SPI0_MCR |=  SPI_MCR_HALT_MASK;

	// flush FIFOs
	SPI0_MCR |= SPI_MCR_CLR_RXF_MASK | SPI_MCR_CLR_TXF_MASK;

	// clear the status bits by writing ones
	SPI0_SR |= SPI_SR_TCF_MASK | SPI_SR_EOQF_MASK | SPI_SR_TFUF_MASK | SPI_SR_TFFF_MASK | SPI_SR_RFOF_MASK | SPI_SR_RFDF_MASK;

	// configure and enable SPI transfers
	SPI0_TCR = 0; // reset transfer counter
	SPI0_MCR &=  ~SPI_MCR_HALT_MASK;


}

void RFM69_Init(){
	// values need to be written to RFM69HCW through SPI
	// SPI initialization MUST take place before chip initialization
	// register values are taken from 'RFM69registers.h'

	// addressing and communication takes 8 bit frames
	// first bit is w/r (1 is write, 0 is read)
	// after w/r bit comes 7 data bits for the address, MSB first

	// enable power for the module

	// use preset values except for following changes
	// set higher bitrate
}

void RFM69_TX(uint8_t tx){
	// NOTE: To end tx the system must leave continuous tx mode using clock disable (?)

	// check chip mode register (REG_OPMODE)
	SPI0_Tx(RFM_READ | REG_OPMODE); // read from RFM OPMODE
	uint8_t status = SPI0_Rx();

	// CONTINUE FROM HERE THIS WON'T WORK YET AS IT WILL JUST WRITE TO THE TX FIFO INSTEAD OF DATA
	// if not in tx mode, set to tx mode
	if(status != RF_OPMODE_TRANSMITTER){
		SPI0_Tx(RFM_WRITE | REG_OPMODE); // write address for OPMODE to RFM
		SPI0_Tx(RF_OPMODE_TRANSMITTER); // write tx mode for OPMODE to RFM

		SPI0_Tx(RFM_WRITE | REG_FIFO); // system will now read continuously
	}


	// push frame to FIFO
}

uint8_t RFM69_RX(uint8_t rx){
	// check chip mode register
	// if register is in rx continue
	// else set to rx mode

	// receive frame from FIFO
	// return frame value
	return 0;
}

void master_init(){
	UART0_Init();
	UART1_Init();
	SPI0_Init();
	//RFM69_Init(); // must always be after the SPI interface has been enabled
}

int main(void){

	master_init();

	uint8_t i;
	while(1){
	for(i = 1; i <= 0xFF; i++){
		UART1_Putstring(i);
		SPI0_Tx(i);
		//UART0_Putstring();
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
