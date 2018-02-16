#ifndef SOURCES_RFM69_RFM69_H
#define SOURCES_RFM69_RFM69_H

void RFM69_TX(uint8_t REG, uint8_t tx_byte);

uint8_t RFM69_RX(uint8_t REG);

void RFM69_Init();

void RFM69_SET_MODE(uint8_t mode);

void RFM69_SEND();

void RFM69_RECEIVE();

#endif // SOURCES_RFM69_RFM69_H
