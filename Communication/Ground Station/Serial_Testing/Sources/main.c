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

// initialize the UART system
void UART0_Init(){
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

// setup UART communication
void UART0_Com(){

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
	masterInit();


	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
