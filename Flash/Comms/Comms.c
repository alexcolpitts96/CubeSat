/* High Level Communication Functions
 *
 * Alex Colpitts
 *
 * Feb 19, 2018
 */

#include <string.h>
#include <stdio.h>
#include "stdlib.h"
#include "string.h"
#include "math.h"

// SDK Included Files
#include "board.h"
#include "main.h"
#include "fsl_debug_console.h"
#include "fsl_device_registers.h"
#include "SSD_FTFx.h"
#include "flash_demo.h"
#include "fsl_interrupt_manager.h"



#include "../RFM69/RFM69_driver.h"
#include "../GPIO/gpio.h"
#include "../FTM/FTM_driver.h" // needed for timeout capability
#include "../GPIO/gpio.h" // included in RFM69 driver
#include "../UART1/UART1_driver.h"
#include "Comms.h"
#include "math.h"
#include "../Camera/camera.h"

////////////////////////////////// GroundStation Functions //////////////////////////////////

// request packet number for satellite to transmit, p[0] is LSB, p[1] is MSB for request -------------------- UNTESTED
void packetRequest(uint8_t *p, uint32_t block) {
	uint8_t handshake = 0;
	uint8_t timeout = 0;

	///*// wait for contact to be made with the satellite
	while (!handshake) {

		// load in block number
		memset(p, 0, sizeof(uint8_t) * PACKET_SIZE);
		p[0] = 0xFF & block; // simply mask
		p[1] = 0xFF & (block >> 8); // shift down and mask
		p[2] = 0xFF & (block >> 16);

		// send block request packet
		RFM69_SEND(p);

		// clear buffer
		memset(p, 0, sizeof(uint8_t) * PACKET_SIZE);

		///* receive packet, able to timeout
		timeout = RFM69_RECEIVE_TIMEOUT(p);

		// ensure timeout hasn't happened
		if (timeout == 1) {
			handshake = 1;
		}

		// if timed out
		else {
			handshake = 0;
		}
		//*/
	}

	// vomit the data over UART
	for (int j = 0; j < PACKET_SIZE; j++) {
		//putty_putchar(p[j]);
		PRINTF("%c", p[j]);
	}
}

// get satellite to start transmitting from p, receives image block count into p
uint32_t txStart(uint8_t *p) {
	uint8_t handshake = 0;
	uint8_t timeout = 0;

	memset(p, 0, sizeof(uint8_t) * PACKET_SIZE);

	///*// wait for contact to be made with the satellite
	while (!handshake) {
		// transmit start sequence
		memcpy((uint8_t *) p, &start_command, sizeof(start_command));

		// send start sequence packet
		RFM69_SEND(p);
		//putty_putchar('t');

		// clear buffer
		memset(p, 0, sizeof(uint8_t) * PACKET_SIZE);

		// receive confirmation signal, able to timeout
		timeout = RFM69_RECEIVE_TIMEOUT(p);

		// ensure timeout hasn't occurred
		if (timeout == 1) {
			handshake = 1;
		}

		// if timed out
		else {
			handshake = 0;
		}
	}

	// read out the image size from p and return it
	return (p[0]) | (p[1] << 8) | (p[2] << 16);
}

//////////////////////////////////// Satellite Functions ////////////////////////////////////

// transmit requested block from buffer (camera) using buffer (p) to transmit
//int transmitPacket(uint8_t *p, uint8_t **image) {
int transmitPacket(uint8_t *p, uint8_t *camera, uint8_t *image) {
	uint8_t packet_request = 0; // 0 when no request, 1 when contacted by ground station
	uint32_t block_number;
	int zero_counter = 0;
	//uint8_t timeout;

	// clean buffer
	memset(p, 0, sizeof(uint8_t) * PACKET_SIZE);

	// wait for packet to be requested
	while (!packet_request) {
		// receive packet request
		RFM69_RECEIVE(p);

		// return 0 if stop command received
		if ((memcmp(&stop_command, p, sizeof(uint8_t) * PACKET_SIZE) == 0)) {
			return 0;
		}

		// start where the packet number isn't
		for (int i = 3; i < PACKET_SIZE; i++) {
			if (p[i] == 0) {
				zero_counter++;
			}
		}

		// ensure it is a good request
		if (zero_counter == PACKET_SIZE - 3) {
			packet_request = 1;
		}
	}

	///*
	// determine the block number
	block_number = (p[2] << 16) | (p[1] << 8) | (p[0]);

	/*// read the correct block into the buffer CAMERA
	for (uint32_t i = 0; i < PACKET_SIZE; i++) {
		camera[i] = image[block_number * PACKET_SIZE + i];
		//putty_putchar(camera[i]);
	}
	//*/

	/*
	 for (int i = 0; i < 10; i++) {
	 camera[i] = cam_reg_read(0x3D);
	 }
	 //*/

	// transmit packet
	RFM69_SEND(image+(block_number * PACKET_SIZE));

	// return 1 if packet was transmitted and no stop requested
	return 1;
}

// transmit image size in blocks to the ground station
void imageSize(uint8_t *p, int fifo_length) {
	uint8_t packet_request = 0; // 0 when no request, 1 when contacted by ground station
	uint8_t block_number[3]; // 3 uint8_t will give 24 bits
	//uint8_t timeout;

// clean buffer
	memset(p, 0, sizeof(uint8_t) * PACKET_SIZE);

// wait for packet to be requested
	while (!packet_request) {
		// receive packet request
		RFM69_RECEIVE(p);
		//timeout = RFM69_RECEIVE_TIMEOUT(p);

		// check if packet is start signal, ensure no timeout
		//if ((strcmp((char *) &start_command, (char *) p) == 0)) {
		if ((memcmp(&start_command, p, sizeof(uint8_t) * PACKET_SIZE) == 0)) {
			packet_request = 1;
		}
	}

// NOTE: block_number[0] is LSB, block_number[2] is MSB
	block_number[0] = fifo_length & 0xFF;
	block_number[1] = (fifo_length >> 8) & 0xFF;
	block_number[2] = (fifo_length >> 16) & 0xFF;

// prepare packet
	memset(p, 0, sizeof(uint8_t) * PACKET_SIZE);
	memcpy((uint8_t *) p, &block_number, sizeof(uint8_t) * 3);

// transmit packet multiple times
	//RFM69_SEND_TIMEOUT(p);
	RFM69_SEND(p);
}

