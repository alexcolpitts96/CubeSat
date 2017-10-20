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

/*	Title: 			Serial Testing
 *
 * 	Description: 	Serial communication testing between FRDM-K64F and Arduino UNO
 * 					This will eventually be updated to work with Parallax RF Equipment
 *
 * 	Author: 		Alex Colpitts
 *
 * 	Date: 			October 19, 2017
 */

#include "fsl_device_registers.h"
#include "stdio.h"
#include "math.h"
#include "string.h"

// initialize the UART system, 9600 baud, no parity, 1 stop bit, no flow control
void UART0_Init(){
	// calculate baud rate register value using ((21000*1000)/(baud_rate * 16))

	// clock enables
	SIM_SCGC4 |= SIM_SCGC4_UART0_MASK; // UART0 clock enabled
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; // GPIO Port B, used by UART0

	// port control mux for Port B to UART0
	PORTB_PCR16 |= PORT_PCR_MUX(3); // RX, B16
	PORTB_PCR17 |= PORT_PCR_MUX(3); // TX, B17

	// control registers and baud rate
	UART0_C1 |= 0x00; 		// setting up control register 1 for UART0
	UART0_C2 |= 0xCC; 		// setting up control register 2 for UART0
	UART0_C3 |= 0x03; 		// setting up control register 3 for UART0
	UART0_BDL |= 0x89;		// setting baud rate for UART0 to 9600
}

// print a string of characters one char at a time
void UART0_Print(char display[]){
	int i = 0;

	while(display[i]){
		UART0_Putchar(display[i]);
		i++;
	}
}

uint8_t UART0_Getchar(){
	// wait until character has been received, flag checks for receive data register full flag to go to 1
	while(!(UART0_S1 & UART_S1_RDRF_MASK));

	return UART0_D; // return the UART0 data register
}

// print a single character
void UART0_Putchar (char display_value){
	int tmp;
	tmp = UART0_S1 & 0x80;

	while(!tmp){
		tmp = UART0_S1 & 0x80;
	}

	UART0_D = display_value;
}

// read in UART data until "start\r" has been detected
int UART0_Tx_Begin(){
	int i = 0;

	char start_code[6] = "start\r";
	char buffer[100]; // read data from UART0
	char temp_read[6];

	for (i = 0; i < 100; i++){

		// read in the character from the UART interface
		buffer[i] = UART0_Getchar();

		// if a new line or carriage return is entered check the last character for start
		if((buffer[i] == '\r') || (buffer[i] == '\n')){

			//////////////////////////////////////////////////////////////////////////////////////////////work from here
		}
	}

	// return 1 if the start command has been found
	return 1;
}

// SPI
void SPI_Init(){

}


// CUSTOM
void CUSTOM_Init(){

}


// Master Init Function
void masterInit(){
	UART0_Init();
	SPI_Init();
	CUSTOM_Init();
}

int main(void)
{
	// initialize all modules
	masterInit();

	char message[20] = "\ntest \r";
	char temp;

	for(;;){
		// receive character
		temp = UART0_Getchar();

		// display the character
		UART0_Putchar(temp);
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
