/* High Level Communication Functions
 *
 * Alex Colpitts
 *
 * Feb 19, 2018
 */

#ifndef SOURCES_COMMS_H
#define SOURCES_COMMS_H

#define MAX_PACKET_SIZE 66 // limited by RFM69HCW FIFO

#include "../RFM69/RFM69_driver.h"

const uint8_t start_command[PACKET_SIZE] = "start";
const uint8_t stop_command[PACKET_SIZE] = "stop";

////////////////////////////////// GroundStation Functions //////////////////////////////////

// request packet number for satellite to transmit, p[0] is LSB, p[1] is MSB for request -------------------- UNTESTED
void packetRequest(uint8_t *p, uint32_t block);

// get satellite to start transmitting from p, receives image block count into p
uint32_t txStart(uint8_t *p);

//////////////////////////////////// Satellite Functions ////////////////////////////////////

// transmit requested block from storage (s) using buffer (p) to transmit -------------------------------- UNTESTED
uint32_t transmitPacket(uint8_t *p, uint8_t *camera, int last_packet);

// transmit image size in blocks to the ground station
void imageSize(uint8_t *p, int fifo_length);

#endif // SOURCES_COMMS_H
