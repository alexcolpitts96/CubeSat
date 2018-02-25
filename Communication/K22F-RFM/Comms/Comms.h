/* High Level Communication Functions
 *
 * Alex Colpitts
 *
 * Feb 19, 2018
 */

#ifndef SOURCES_COMMS_H
#define SOURCES_COMMS_H

////////////////////////////////// GroundStation Functions //////////////////////////////////

// request packet number for satellite to transmit, p[0] is LSB, p[1] is MSB for request -------------------- UNTESTED
void packetRequest(uint8_t *p, uint16_t block);

// get satellite to start transmitting from p, receives image block count into p
void txStart(uint8_t *p);

//////////////////////////////////// Satellite Functions ////////////////////////////////////

// transmit requested block from storage (s) using buffer (p) to transmit -------------------------------- UNTESTED
void transmitPacket(uint8_t *p, uint8_t **s);

// transmit image size in blocks to the ground station
void imageSize(uint8_t *p, uint16_t image_size);

#endif // SOURCES_COMMS_H
