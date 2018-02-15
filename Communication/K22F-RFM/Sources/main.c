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
#define PACKET_SIZE 66
#define COMMAND_SIZE 5
#define MAX_PACKET_SIZE 66 // limited by RFM69HCW FIFO

// cubesat commands
const uint8_t start_command[] = "start packet transmission"; // might need to be changed for packet length


void master_init(){
	UART0_Init();

	UART1_putty_init();
	SPI0_Init(16);
	RFM69_DIO0_Init();
	RFM69_Init(); // must always be after the SPI interface has been enabled
}

int main(void){
	int i, mode_select;
	uint8_t *p;
	p = (uint8_t *) calloc(MAX_PACKET_SIZE, sizeof(uint8_t));

	// init all the modules needed
	master_init();

	// 1 is transmit
	// 2 is receive
	// 3 is ground station test
	// 4 is satellite test
	// 5 is text transmit relay (other radio must be in 2)
	mode_select = 3;

	//start as transmitter /////////////////////////////////////////////////////////////////////////////////////////
	while(mode_select == 1){

		// clean the buffer
		memset(p, 0, sizeof(uint8_t)*PACKET_SIZE);

		// copy in the data of interest, all other data is null
		memcpy((uint8_t *) p, "abcdefghijklmnopqrstuvwxyz", sizeof("abcdefghijklmnopqrstuvwxyz"));
		//memcpy((uint8_t *) p, "quickbrownfox", sizeof("quickbrownfox"));

		RFM69_SEND(p);

		for(i = 0; i < PACKET_SIZE; i++){
			putty_putchar(p[i]);
		}

		//putty_putchar('\n');
		//putty_putchar('\r');

		// clean the buffer
		memset(p, 0, sizeof(uint8_t)*PACKET_SIZE);
	}

	//start as receiver /////////////////////////////////////////////////////////////////////////////////////////
	while(mode_select == 2){

		memset(p, 0, sizeof(uint8_t)*PACKET_SIZE);
		RFM69_RECEIVE(p);

		for(i = 0; i < PACKET_SIZE; i++){
			putty_putchar(p[i]);
		}

		//putty_putchar('\n');
		//putty_putchar('\r');

		memset(p, 0, sizeof(uint8_t)*PACKET_SIZE);
	}

	// setup for ground station /////////////////////////////////////////////////////////////////////////////////////////
	while(mode_select == 3){

		uint8_t handshake = 0; // 0 when no contact, 1 when contacted by satellite

		// wait for contact to be made with the satellite
		while(!handshake){
			// transmit start sequence


			// receive confirmation, set handshake as needed


		}

		// receive image packet, loop continues


		// push image to putty log

	}

	// setup for satellite /////////////////////////////////////////////////////////////////////////////////////////
	while(mode_select == 4){

		uint8_t packet_request = 0; // 0 when no request, 1 when contacted by ground station

		// wait for packet to be requested
		while(!packet_request){
			// receive packet


			// check is packet is start signal, set packet_request as needed


		}

		// transmit packet


	}

	// start as text relay /////////////////////////////////////////////////////////////////////////////////////////
	while(mode_select == 5){
		// clear the buffer
		memset(p, 0, sizeof(uint8_t)*PACKET_SIZE);

		// read in data
		i = 0;
		while(i < PACKET_SIZE && p[i-1] != '\r'){ // this makes me feel dirty inside
			p[i] = putty_getchar();
			putty_putchar(p[i]);
			i++;
		}

		putty_putchar('\n');
		putty_putchar('\r');

		// transmit packet
		RFM69_SEND(p);
	}
}
