#include "fsl_device_registers.h"
#include "stdio.h"
#include "stdlib.h"

#define SPI_PUSHR_PCS0_ON 0x10000 // used by SPI0_TX

// initialize the SPI0 module with a specific frame size
void SPI0_Init(int frame_size);

// prepare SPI0 to transmit
void SPI0_Prep();

// receive a frame
uint16_t SPI0_RX();

// transmit a frame
void SPI0_TX(uint16_t tx_data);
