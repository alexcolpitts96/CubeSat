/* RFM69HCW Driver
 *
 * Alex Colpitts
 *
 */

#ifndef SOURCES_RFM69_RFM69_H
#define SOURCES_RFM69_RFM69_H

void RFM69_TX(uint8_t REG, uint8_t tx_byte);

uint8_t RFM69_RX(uint8_t REG);

void RFM69_Init();

// clear the FIFO as needed
void RFM69_CLEAR_FIFO();

// pass in mode to have the system set into it
void RFM69_SET_MODE(uint8_t mode);

// send buffer, will be seq_length bytes (11 bytes), see SEQ_LEN definition
void RFM69_SEND(uint8_t *buffer);

void RFM69_SEND_TIMEOUT(uint8_t *buffer);

// check if payloadready flag has been set, return 1 if set
uint8_t RFM69_PL_RD();

// receive from the RFM69 fifo
void RFM69_RECEIVE(uint8_t *buffer);

// try to receive a packet, return -1 and clear buffer if no packet received after some time T
// timeout variable will be 0 when no timeout, 1 when timed out
uint8_t RFM69_RECEIVE_TIMEOUT(uint8_t *buffer);

#endif // SOURCES_RFM69_RFM69_H
