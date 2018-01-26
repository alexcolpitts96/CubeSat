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
#include "SPI0_driver.h"
#include "UART0_driver.h"
#include "UART1_driver.h"

// definitions
#define RFM_WRITE 0x80
#define RFM_READ 0x00

void RFM69_Init(){
	uint8_t temp;
	SPI0_Prep(); // prepare SPI

	// see page 60 in RFM user manual for register details

	// module set to standby by default, ok for configuration

	// set to continuous mode, fsk (default)
	SPI0_TX((RFM_WRITE | REG_DATAMODUL) << 8 | RF_DATAMODUL_DATAMODE_CONTINUOUS);

	// set bitrate of the communication link to 300 kbps
	//SPI0_TX((RFM_WRITE | REG_BITRATEMSB) << 8 | RF_BITRATEMSB_300000);
	//SPI0_TX((RFM_WRITE | REG_BITRATELSB) << 8 | RF_BITRATELSB_300000);

	// must follow: frequency deviation + bitrate/2 <= 500kHz, f_dev = 200kHz
	//SPI0_TX((RFM_WRITE | REG_FDEVMSB) << 8 | RF_FDEVMSB_200000);
	//SPI0_TX((RFM_WRITE | REG_FDEVLSB) << 8 | RF_FDEVLSB_200000);

	// leave REG_FRFxxx to default values (915 MHz)

	// Calibrate RC Oscillator
	SPI0_TX((RFM_WRITE | REG_OSC1) << 8 | RF_OSC1_RCCAL_START);
	temp = 0;
	while(!temp){
		SPI0_TX(RFM_READ | REG_OSC1); // make read request
		temp = SPI0_RX() & RF_OSC1_RCCAL_DONE; // mask all bits except for RCCAL_DONE
	}

	// leave RegAfcCtrl as default

	// ignore RegListenX for now

	// set power level to 0 (dBm)
	SPI0_TX((RFM_WRITE | REG_PALEVEL) << 8 | RF_PALEVEL_PA0_ON | RF_PALEVEL_PA1_OFF | RF_PALEVEL_PA2_OFF | RF_PALEVEL_OUTPUTPOWER_10010);

}

void RFM69_TX_Prep(){
	// change operation mode to transmit
	SPI0_TX((RFM_WRITE | REG_OPMODE) << 8 | RF_OPMODE_TRANSMITTER);

	// maybe look at configuring SPI to be 8 bits after this poin
}

void RFM69_RX_Prep(){
	// change operation mode to receive
	SPI0_TX((RFM_WRITE | REG_OPMODE) << 8 | RF_OPMODE_RECEIVER);
	// maybe look at configuring SPI to be 8 bits after this point
}

void RFM69_TX(uint8_t tx_byte){
	SPI0_TX((RFM_WRITE | REG_FIFO) << 8 | tx_byte); // want to write to fifo for TX
}

uint8_t RFM69_RX(){
	SPI0_TX(RFM_READ | REG_FIFO); // want to read from fifo for RX
	return SPI0_RX();
}

void master_init(){
	UART0_Init();
	UART1_Init();
	SPI0_Init(16);
	RFM69_Init(); // must always be after the SPI interface has been enabled
}

void TX_TEST(){
	uint8_t i;

	// prepare for wireless transmission
	RFM69_TX_Prep();

	while(1){ // loop through for visualisation purposes
		for(i = 0; i < 0xFF; i++){
			RFM69_TX(i); // transmit incrementing numbers
		}
	}
}

void RX_TEST(){
	uint8_t temp;

	RFM69_RX_Prep();

	while(1){
		temp = RFM69_RX();
	}
}

int main(void){
	uint8_t i, temp;

	// note max current draw for board is 120 mA, keep below that

	// look at SPI0_CTAR0 PASC to change the time before the CS goes high again

	// init all the modules needed
	master_init();

	while(1){
		//for(i = 0; i < 0xFF; i++){
			SPI0_TX(RFM_READ | REG_FRFMSB << 8);
			temp = SPI0_RX();
		//}
	}
	//RX_TEST();
	//TX_TEST();
}
