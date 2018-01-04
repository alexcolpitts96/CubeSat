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
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK; // enable GPIO E

	// mux pins for GPIO and UART
	PORTE_PCR3 |= PORT_PCR_MUX(3); // RX, E3
	PORTE_PCR4 |= PORT_PCR_MUX(3); // TX, E4

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
void UART1_Putstring() {
	int i = 0;
	char welcome[80] = "Welcome to our program\0";

	while(welcome[i]){
		UART1_Putchar(welcome[i]);
		i++;
	}
}

void SPI0_Init(){
	// initialize the SPI bus
	// CS -> D0
	// CLK -> D1
	// MOSI -> D2
	// MISO -> D3

	// clock enables for GPIO and SPI
	SIM_SCGC6 |= SIM_SCGC6_SPI0_MASK; // enable SPI0
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK; // enable GPIO D

	// mux pins for SPI and UART
	PORTD_PCR0 |= PORT_PCR_MUX(2); // Chip select
	PORTD_PCR1 |= PORT_PCR_MUX(2); // Clock
	PORTD_PCR2 |= PORT_PCR_MUX(2); // MOSI
	PORTD_PCR3 |= PORT_PCR_MUX(2); // MISO

	// configure SPI0
	SPI0_MCR |= SPI_MCR_MSTR_MASK | SPI_MCR_CONT_SCKE_MASK; // master, continuous clock
	SPI0_CTAR0 |= SPI_CTAR_FMSZ(8) | SPI_CTAR_BR(0xC); // 8 bit frames, 4096 baud rate clock
}

// read from SPI0 MISO and return the value
// NOTE: This does not check if RFM is ready
uint8_t SPI0_Rx(){
	// mask the received data to be only 8 bits
	uint8_t temp = SPI0_POPR & 0xFF;

	// there wasn't any way to check RX complete flag?
	return temp;
}

// write a byte to SPI0 MOSI
// NOTE: This does not check if RFM is ready
void SPI0_Tx(uint8_t tx_data){
	// wait until the transfer has completed, flag goes to 1
	//while(SPI0_SR | SPI_SR_TCF_MASK){
	//	SPI0_SR |= SPI_SR_TCF_MASK; // reset flag with a 1
	//}

	// transmit the next frame to the
	SPI0_PUSHR |= SPI_PUSHR_TXDATA(tx_data);
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
	//SPI0_Init();
	//RFM69_Init(); // must always be after the SPI interface has been enabled
}

// hand function test information to write and have returned
uint8_t SPI_Test(uint8_t test){
	SPI0_Tx(test); // output the data
	uint8_t temp = SPI0_Rx() & 0xFF; // returned data

	if(temp == test){
		return 1;
	}
	else{
		return 0;
	}
}


int main(void)
{
	master_init();

	for(;;){
		UART0_Putstring();
		UART1_Putstring();
		}
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
