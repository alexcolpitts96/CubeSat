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
#include "../RFM69/RFM69registers.h"
#include "../RFM69/RFM69_driver.h"
//#include "SPI0_driver.h" // contained in RFM69_driver.h
#include "../UART0/UART0_driver.h"
#include "../UART1/UART1_driver.h"
//#include "../GPIO/gpio.h"

// definitions
#define RFM_WRITE 0x80
#define RFM_READ 0x00
#define RFM_SAFE_BTYE 0xFF // this is a safe register to address as it doesn't exist
#define MAX_STRING_LENGTH 60

#define SEQ_LEN 13

void master_init(){
	UART0_Init();

	UART1_putty_init();
	SPI0_Init(16);
	RFM69_DIO0_Init();
	RFM69_Init(); // must always be after the SPI interface has been enabled
}

int main(void){
	char buffer[MAX_STRING_LENGTH];

	int i, mode_select;
	uint8_t *p;
	p = (char *) calloc(MAX_STRING_LENGTH, sizeof(char));

	//uint8_t buffer[MAX_STRING_LENGTH];// = "test";
	//p = &buffer;

	// init all the modules needed
	master_init();

	// 1 is transmit
	// 2 is receive
	mode_select = 2;

	//start as transmitter /////////////////////////////////////////////////////////////////////////////////////////
	while(mode_select == 1){
		//temp = 0;

		/* get the message to send
		while(temp != '@' && i < MAX_STRING_LENGTH){
			temp = putty_getchar(); // store in buffer
			p[i] = temp;
			putty_putchar(temp); // echo to console
			i++;
		}
		//*/
		memset(p, '\0', sizeof(uint8_t)*MAX_STRING_LENGTH);
		strcpy(p, "string");

		RFM69_SEND(p);
		putty_putchar('s');
		putty_putchar('\n');
		putty_putchar('\r');

		// clean the buffer
		memset(p, '\0', sizeof(uint8_t)*MAX_STRING_LENGTH);
	}

	//start as receiver /////////////////////////////////////////////////////////////////////////////////////////
	while(mode_select == 2){
		memset(p, '\0', sizeof(uint8_t)*MAX_STRING_LENGTH);
		RFM69_RECEIVE(p);

		for(i = 0; i < MAX_STRING_LENGTH; i++){
			putty_putchar(p[i]);
			buffer[i] = p[i];
		}

		putty_putchar('\n');
		putty_putchar('\r');

		memset(p, '\0', sizeof(uint8_t)*MAX_STRING_LENGTH);
	}
}
