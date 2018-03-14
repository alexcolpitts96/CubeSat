/* High Level Communication Functions
 *
 * Alex Colpitts
 *
 * Feb 19, 2018
 */

#ifndef SOURCES_COMMS_H
#define SOURCES_COMMS_H

#include "../RFM69/RFM69_driver.h"

const uint8_t start_command[PACKET_SIZE] = "start";
const uint8_t stop_command[PACKET_SIZE] = "stop";
const uint8_t resend_command[PACKET_SIZE] = "resend";
const uint8_t next_command[PACKET_SIZE] = "next";

////////////////////////////////// GroundStation Functions //////////////////////////////////

// request packet number for satellite to transmit, p[0] is LSB, p[1] is MSB for request -------------------- UNTESTED
void packetRequest(uint8_t *p, uint32_t block);

// get satellite to start transmitting from p, receives image block count into p
uint32_t txStart(uint8_t *p);

//////////////////////////////////// Satellite Functions ////////////////////////////////////

// transmit requested block from storage (s) using buffer (p) to transmit -------------------------------- UNTESTED
int transmitPacket(uint8_t *p, uint8_t *camera, uint8_t *image);
//uint32_t transmitPacket(uint8_t *p, uint8_t *camera, uint32_t last_block);

// transmit image size in blocks to the ground station
void imageSize(uint8_t *p, int fifo_length);

#endif // SOURCES_COMMS_H
