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

#define PACKET_SIZE 66
#define MAX_PACKET_SIZE 66 // limited by RFM69HCW FIFO

const uint8_t start_command[PACKET_SIZE] = "start packet transmission";

////////////////////////////////// GroundStation Functions //////////////////////////////////

// request packet number for satellite to transmit
void packetRequest(uint8_t *p, int block);

// get satellite to start transmitting from p, receives image block count into p
void txStart(uint8_t *p){
	uint8_t handshake = 0;
	uint8_t timeout = 0;

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

		// ensure timeout hasn't occured
		if(timeout == 1){
			handshake = 1;
		}

		// if timed out
		else{
			handshake = 0;
		}
	}
}


//////////////////////////////////// Satellite Functions ////////////////////////////////////

// transmit block n
void transmitPacket(uint8_t *p, int block);

// transmit image size in blocks
void imageSize(uint8_t *p, uint16_t image_size){
	uint8_t packet_request = 0; // 0 when no request, 1 when contacted by ground station
	uint8_t block_number[2]; // 2 uint8_t will give 16 bits effectively
	uint8_t i;

	// clean buffer
	memset(p, 0, sizeof(uint8_t)*PACKET_SIZE);

	// wait for packet to be requested
	while(!packet_request){
		// receive packet request
		RFM69_RECEIVE(p);

		// check if packet is start signal, ensure no timeout
		if((strcmp(&start_command, p) == 0)){
			packet_request = 1;
		}
	}

	// NOTE: block_number[0] is LSB, block_number[1] is MSB
	// mask 8 LSB into block_number
	block_number[0] = image_size & 0xFF;

	// mask 8 MSB bits and shift down into block_number
	block_number[1] = (image_size & 0xFF00) >> 8;

	// prepare packet
	memset(p, 0, sizeof(uint8_t)*PACKET_SIZE);
	memcpy((uint8_t *) p, &block_number, sizeof(uint8_t)*2);

	// transmit packet multiple times
	RFM69_SEND_TIMEOUT(p);
}

