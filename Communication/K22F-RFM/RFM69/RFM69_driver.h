// custom libraries
//#include <SPI0_driver.c>
#include "../SPI0/SPI0_driver.h"
#include "RFM69registers.h"
#include "../GPIO/gpio.h"

// definitions
#define RFM_WRITE 0x80
#define RFM_READ 0x00
#define LISTEN_ABORT_IGNORE 0xE3
#define SEQ_LEN 13
#define networkID 100

void RFM69_TX(uint8_t REG, uint8_t tx_byte);

uint8_t RFM69_RX(uint8_t REG);

void RFM69_Init();

void RFM69_SET_MODE(uint8_t mode);

void RFM69_SEND();

void RFM69_RECEIVE();
