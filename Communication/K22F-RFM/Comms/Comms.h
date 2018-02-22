/* High Level Communication Functions
 *
 * Alex Colpitts
 *
 * Feb 19, 2018
 */

#ifndef SOURCES_COMMS_H
#define SOURCES_COMMS_H

////////////////////////////////// GroundStation Functions //////////////////////////////////

// request packet number for satellite to transmit
void packetRequest(uint8_t *p, int block);

// get satellite to start transmitting from p, receives image block count into p
void txStart(uint8_t *p);

//////////////////////////////////// Satellite Functions ////////////////////////////////////

// transmit block n
void transmitPacket(uint8_t *p, int block);

// transmit image size in blocks
void imageSize(uint8_t *p, uint16_t image_size);

#endif // SOURCES_COMMS_H
