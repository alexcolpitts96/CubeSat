/* High Level Communication Functions
 *
 * Alex Colpitts
 *
 * Feb 19, 2018
 */

#include "fsl_device_registers.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "../RFM69/RFM69_driver.h"
#include "../GPIO/gpio.h"
#include "../FTM/FTM_driver.h" // needed for timeout capability
#include "../GPIO/gpio.h" // included in RFM69 driver
#include "../UART1/UART1_driver.h"

#define PACKET_SIZE 10
#define MAX_PACKET_SIZE 66 // limited by RFM69HCW FIFO

const uint8_t start_command[PACKET_SIZE] = "start packet transmission";

////////////////////////////////// GroundStation Functions //////////////////////////////////

// request packet number for satellite to transmit, p[0] is LSB, p[1] is MSB for request -------------------- UNTESTED
void packetRequest(uint8_t *p, uint16_t block) {
	uint8_t handshake = 0;
	uint8_t timeout = 0, i;
	uint8_t temp[20]; // for printing block number

	///*// wait for contact to be made with the satellite
	while (!handshake) {

		// load in block number
		memset(p, 0, sizeof(uint8_t) * PACKET_SIZE);
		p[0] = 0xFF & block; // simply mask
		p[1] = 0xFF & (block >> 8); // shift down and mask

		// send block request packet
		RFM69_SEND(p);

		// clear buffer
		memset(p, 0, sizeof(uint8_t) * PACKET_SIZE);

		// receive confirmation signal, able to timeout
		timeout = RFM69_RECEIVE_TIMEOUT(p);

		// ensure timeout hasn't occured
		if (timeout == 1) {
			handshake = 1;
		}

		// if timed out
		else {
			handshake = 0;
		}
	}

	/* print the block number first to the data sheet
	sprintf((char *) temp, "block number %d", block); // warning is since it is uint8_t not char (equivalent)
	for (i = 0; i < PACKET_SIZE; i++) {
		putty_putchar(temp[i]);
	}
	//*/

	//putty_putstr("\r\n");

	// vomit the data over UART
	for (i = 0; i < PACKET_SIZE; i++) {
		putty_putchar(p[i]);
	}
}

// get satellite to start transmitting from p, receives image block count into p
void txStart(uint8_t *p) {
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
}

//////////////////////////////////// Satellite Functions ////////////////////////////////////

// transmit requested block from storage (s) using buffer (p) to transmit
void transmitPacket(uint8_t *p, uint8_t **s) {
	uint8_t packet_request = 0; // 0 when no request, 1 when contacted by ground station
	uint8_t i;
	uint8_t packet_zeros;
	uint16_t block_number;

	// clean buffer
	memset(p, 0, sizeof(uint8_t) * PACKET_SIZE);

	// wait for packet to be requested
	while (!packet_request) {
		// receive packet request
		RFM69_RECEIVE(p);

		// check how many zeros are in the packet excluding the first two entries
		packet_zeros = 0;
		for (i = 2; i < PACKET_SIZE; i++) {
			if (p[i] == 0) {
				packet_zeros++;
			}
		}

		// check if all bytes are 0 except for the first 2 (0, 1)
		if (packet_zeros == PACKET_SIZE - 2) {
			packet_request = 1;
		}
	}

	// determine the block number
	block_number = (p[1] << 8) | (p[0]);

	// read block from s into p
	memset(p, 0, sizeof(uint8_t) * PACKET_SIZE);
	memcpy(p, s[block_number], sizeof(uint8_t) * PACKET_SIZE);

	// transmit packet multiple times
	//RFM69_SEND_TIMEOUT(p);
	RFM69_SEND(p);
}

// transmit image size in blocks to the ground station
void imageSize(uint8_t *p, uint16_t image_size) {
	uint8_t packet_request = 0; // 0 when no request, 1 when contacted by ground station
	uint8_t block_number[2]; // 2 uint8_t will give 16 bits effectively

	// clean buffer
	memset(p, 0, sizeof(uint8_t) * PACKET_SIZE);

	// wait for packet to be requested
	while (!packet_request) {
		// receive packet request
		RFM69_RECEIVE(p);

		// check if packet is start signal, ensure no timeout
		if ((strcmp((char *) &start_command, (char *) p) == 0)) {
			packet_request = 1;
		}
	}

	// NOTE: block_number[0] is LSB, block_number[1] is MSB
	// mask 8 LSB into block_number
	block_number[0] = image_size & 0xFF;

	// mask 8 MSB bits and shift down into block_number
	block_number[1] = (image_size & 0xFF00) >> 8;

	// prepare packet
	memset(p, 0, sizeof(uint8_t) * PACKET_SIZE);
	memcpy((uint8_t *) p, &block_number, sizeof(uint8_t) * 2);

	// transmit packet multiple times
	RFM69_SEND_TIMEOUT(p);
}

