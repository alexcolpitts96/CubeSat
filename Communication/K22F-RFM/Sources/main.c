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

// standard libraries
#include "fsl_device_registers.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

// custom libraries
#include "RFM69registers.h"
#include "RFM69_driver.h"
//#include "SPI0_driver.h" // contained in RFM69_driver.h
#include "UART0_driver.h"
#include "UART1_driver.h"

// definitions
#define RFM_WRITE 0x80
#define RFM_READ 0x00
#define RFM_SAFE_BTYE 0xFF

void TX_TEST(){
	uint8_t i;

	RFM69_TX_Prep();

	while(1){
		for(i = 0; i < 0xFF; i++){
			RFM69_TX(i);
		}
	}
}

void RX_TEST(){
	uint16_t temp;

	RFM69_RX_Prep();

	while(1){
		temp = RFM69_RX() & 0xFF; // receive and mask the result
	}


}

void master_init(){
	UART0_Init();
	UART1_Init();
	SPI0_Init(16);
	RFM69_Init(); // must always be after the SPI interface has been enabled
}

int main(void){
	uint16_t i, temp, tx;

	// note max current draw for board is 120 mA, keep below that

	// vary SPI init depending on the task to be completed

	// init all the modules needed
	master_init();

	SPI0_Prep();

	//TX_TEST();

	SPI0_TX(((RFM_WRITE | REG_OPMODE) << 8) | RF_OPMODE_TRANSMITTER);

	while(1){
		for(i = 0; i < 0xFF; i++){
			SPI0_TX(((RFM_WRITE | REG_FIFO) << 8) | i);
		}
	}

}
