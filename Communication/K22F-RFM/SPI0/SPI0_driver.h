#include "fsl_device_registers.h"
#include "stdio.h"
#include "stdlib.h"

#define SPI_PUSHR_PCS0_ON 0x10000 // used by SPI0_TX

void SPI0_Init(int frame_size);

void SPI0_Prep();

uint16_t SPI0_RX();

void SPI0_TX(uint16_t tx_data);
