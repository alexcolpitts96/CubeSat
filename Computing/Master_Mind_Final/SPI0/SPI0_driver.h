#ifndef SOURCES_SPI0_SPI0_H
#define SOURCES_SPI0_SPI0_H

// initialize the SPI0 module with a specific frame size
void SPI0_Init(int frame_size);

// prepare SPI0 to transmit
void SPI0_Prep();

// receive a frame
uint16_t SPI0_RX();

// transmit a frame
void SPI0_TX(uint16_t tx_data);

#endif // SOURCES_SPI0_SPI0_H
