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
#include "../UART1/UART1_driver.h"
#include "../FTM/FTM_driver.h"
#include "../UART0/UART0_driver.h"
#include "../SPI0/SPI0_driver.h"
#include "../GPIO/gpio.h" // included in RFM69 driver

// definitions
#define RFM_WRITE 0x80
#define RFM_READ 0x00
#define RFM_SAFE_BTYE 0xFF // this is a safe register to address as it doesn't exist
#define PACKET_SIZE 66
#define MAX_PACKET_SIZE 66 // limited by RFM69HCW FIFO

// cubesat commands
const uint8_t start_command[PACKET_SIZE] = "start packet transmission"; // might need to be changed for packet length

// for testing purposes
const uint8_t test_data[PACKET_SIZE] = "abcdefghijklmnopqrstuvwxyz1234567890";

void master_init(){
	UART0_Init();
	UART1_putty_init();
	SPI0_Init(16);
	RFM69_DIO0_Init();
	RFM69_Init(); // must always be after the SPI interface has been enabled
	FTM0_init();
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
	// 6 is FTM0 test
	mode_select = 3;

	//start as transmitter /////////////////////////////////////////////////////////////////////////////////////////
	while(mode_select == 1){

		// clean the buffer
		memset(p, 0, sizeof(uint8_t)*PACKET_SIZE);

		// copy in the data of interest, all other data is null
		//memcpy((uint8_t *) p, "abcdefghijklmnopqrstuvwxyz", sizeof("abcdefghijklmnopqrstuvwxyz"));
		//memcpy((uint8_t *) p, "quickbrownfox", sizeof("quickbrownfox"));
		memcpy((uint8_t *) p, &start_command, sizeof(start_command));

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
		uint8_t packet_request = 0;

		memset(p, 0, sizeof(uint8_t)*PACKET_SIZE);
		RFM69_RECEIVE(p);

		if((strcmp(&start_command, p) == 0)){
			packet_request = 1;
		}

		for(i = 0; i < PACKET_SIZE; i++){
			putty_putchar(p[i]);
		}

		putty_putchar('\n');
		putty_putchar('\r');

		memset(p, 0, sizeof(uint8_t)*PACKET_SIZE);
	}

	// setup for ground station /////////////////////////////////////////////////////////////////////////////////////////
	while(mode_select == 3){

		uint8_t handshake = 0; // 0 when no contact, 1 when contacted by satellite
		uint8_t timeout = 0; // will be 0 if timeout, 1 if no timeout

		// clean buffer to prevent false results
		memset(p, 0, sizeof(uint8_t)*PACKET_SIZE);

		///*// wait for contact to be made with the satellite
		while(!handshake){
			// transmit start sequence
			memcpy((uint8_t *) p, &start_command, sizeof(start_command));

			// send start sequence packet
			RFM69_SEND(p);
			//putty_putchar('t');

			// clear buffer
			memset(p, 0, sizeof(uint8_t)*PACKET_SIZE);

			// receive confirmation signal, able to timeout
			timeout = RFM69_RECEIVE_TIMEOUT(p);

			// check if packet was the start reception command and no timeout occurred...
			if(timeout == 1){ // if no timeout occurred
				handshake = 1;
			}

			// if timed out
			else{
				handshake = 0;
			}
		}
		//*/ ability to comment out the above portion of code

		// push image to putty log file
		for(i = 0; i < PACKET_SIZE; i++){
			putty_putchar(p[i]);
		}

		putty_putchar('\n');
		putty_putchar('\r');
	}

	// setup for satellite /////////////////////////////////////////////////////////////////////////////////////////
	while(mode_select == 4){

		uint8_t packet_request = 0; // 0 when no request, 1 when contacted by ground station
		//uint8_t timeout;

		// clean buffer
		memset(p, 0, sizeof(uint8_t)*PACKET_SIZE);

		///*// wait for packet to be requested
		while(!packet_request){
			// receive packet request
			//timeout = RFM69_RECEIVE_TIMEOUT(p);
			RFM69_RECEIVE(p);

			// check is packet is start signal, ensure no timeout
			//if((strcmp(&start_command, p) == 0) && timeout == 1){
			if((strcmp(&start_command, p) == 0)){
				packet_request = 1;
			}
		}
		//*/

		// wait to receive packet
		//RFM69_RECEIVE(p);

		// prepare packet
		memset(p, 0, sizeof(uint8_t)*PACKET_SIZE);
		memcpy((uint8_t *) p, &test_data, sizeof(test_data));

		// transmit packet multiple times
		RFM69_SEND_TIMEOUT(p);
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

	// FTM0 TEST
	while(mode_select == 6){

		// reset counter to 0;
		FTM0_CNT_RESET();

		// run through loop until timeout occurs
		while(!FTM0_WAIT());
		putty_putchar('s');
	}
}
