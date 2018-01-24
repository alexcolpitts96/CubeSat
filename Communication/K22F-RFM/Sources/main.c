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
void UART0_Putchar(char display_char){

	while(!(UART0_S1 & UART_S1_TC_MASK)){} // wait until the tx is ready for next char

	UART0_D = display_char; // when ready send next char
}

// print a single character
void UART1_Putchar(char display_char){

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
void UART1_Putstring(uint8_t num){
	int i = 0;
	char welcome[80] = "Test  \n\0";
	welcome[5] = num;

	while(welcome[i]){
		UART1_Putchar(welcome[i]);
		i++;
	}
}

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
	SPI0_CTAR0 |= SPI_CTAR_FMSZ(frame_size-1) | SPI_CTAR_CPOL_MASK | SPI_CTAR_BR(0x9) | SPI_CTAR_CPHA_MASK; // 16 bit frames, active low clock, ??? baud rate clock, BR factor 2
	SPI0_MCR |= SPI_MCR_MSTR_MASK | SPI_MCR_PCSIS_MASK; // master, CS active low
	SPI0_MCR &= (~SPI_MCR_DIS_RXF_MASK) | (~SPI_MCR_DIS_TXF_MASK); // enable rx and tx FIFOs
	SPI0_MCR &= (~SPI_MCR_MDIS_MASK) & (~SPI_MCR_HALT_MASK); // enable module clock and start transfers
}

// configure SPI to start transfer
void SPI0_Tx_Prep(){

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

// transmit byte after SPI has been configured for tx
void SPI0_Tx(uint16_t tx_data){
	SPI0_PUSHR =  (SPI_PUSHR_PCS0_ON | tx_data); // may need to be changed based on chip, CS0

	// wait for transmission to complete flag to go to 1 (TCF)
	while(!(SPI0_SR & SPI_SR_TCF_MASK));

	// clear status bits, this may not need to be done
	SPI0_SR |= SPI_SR_TFFF_MASK;
}

// MAY NOT BE NEEDED AS IT IS IDENTICAL TO SPI0_Tx_Prep()
void SPI0_Rx_Prep(){
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

uint8_t SPI0_Rx(){
	uint8_t temp;

	// wait for receive fifo drain flag (RFDF) to go to 1
	while(!(SPI0_SR & SPI_SR_RFDF_MASK));

	// save the rx data
	temp = SPI0_POPR;

	// clear RFDF flag
	SPI0_SR |= SPI_SR_RFDF_MASK;

	return temp;
}

void RFM69_Init(){
	int temp;
	SPI0_Tx_Prep(); // prepare SPI

	// see page 60 in RFM user manual for register details

	// module set to standby by default, ok for configuration

	// set to continuous mode, fsk (default)
	SPI0_Tx((RFM_WRITE | REG_DATAMODUL) << 8 | RF_DATAMODUL_DATAMODE_CONTINUOUS);

	// set bitrate of the communication link to 300 kbps
	SPI0_Tx((RFM_WRITE | REG_BITRATEMSB) << 8 | RF_BITRATEMSB_300000);
	SPI0_Tx((RFM_WRITE | REG_BITRATELSB) << 8 | RF_BITRATELSB_300000);

	// must follow: frequency deviation + bitrate/2 <= 500kHz, f_dev = 200kHz
	SPI0_Tx((RFM_WRITE | REG_FDEVMSB) << 8 | RF_FDEVMSB_200000);
	SPI0_Tx((RFM_WRITE | REG_FDEVLSB) << 8 | RF_FDEVLSB_200000);

	// leave REG_FRFxxx to default values (915 MHz)

	/*
	// start OSC1 calibration sequence
	SPI0_Tx((RFM_WRITE | REG_OSC1) << 8 | RF_OSC1_RCCAL_START);

	// check OSC1 calibration to see if completed
	temp = 0;
	SPI0_Tx((RFM_WRITE | REG_OSC1) << 8);
	while(!temp){
		SPI0_Tx((RFM_READ | REG_OSC1) << 8);
		temp = SPI0_Rx() & RF_OSC1_RCCAL_DONE;
	}
	*/
}

void RFM69_TX(uint8_t tx){

}

uint8_t RFM69_RX(uint8_t rx){
	return 0xFF; // dummy return
}

void master_init(){
	UART0_Init();
	UART1_Init();
	SPI0_Init(16);
	//RFM69_Init(); // must always be after the SPI interface has been enabled
}

int main(void){
	// note max current draw for board is 120 mA, keep below that

	master_init();

	SPI0_Tx_Prep(); // configure SPI

	while(1){
		RFM69_Init();
	}
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
