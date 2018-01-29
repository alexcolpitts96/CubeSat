#include "fsl_device_registers.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

// custom libraries
#include "RFM69registers.h"
#include "SPI0_driver.h"
#include "UART0_driver.h"
#include "UART1_driver.h"

// definitions
#define RFM_WRITE 0x80
#define RFM_READ 0x00

void RFM69_Init(){
	uint8_t temp;
	SPI0_Prep(); // prepare SPI

	// see page 60 in RFM user manual for register details

	// module set to standby by default, ok for configuration

	// set to continuous mode, fsk (default)
	SPI0_TX(RFM_WRITE | REG_DATAMODUL);
	SPI0_TX(RF_DATAMODUL_DATAMODE_CONTINUOUS);

	// set bitrate of the communication link to 300 kbps
	//SPI0_TX((RFM_WRITE | REG_BITRATEMSB) << 8 | RF_BITRATEMSB_300000);
	//SPI0_TX((RFM_WRITE | REG_BITRATELSB) << 8 | RF_BITRATELSB_300000);

	// must follow: frequency deviation + bitrate/2 <= 500kHz, f_dev = 200kHz
	//SPI0_TX((RFM_WRITE | REG_FDEVMSB) << 8 | RF_FDEVMSB_200000);
	//SPI0_TX((RFM_WRITE | REG_FDEVLSB) << 8 | RF_FDEVLSB_200000);

	// leave REG_FRFxxx to default values (915 MHz)

	// Calibrate RC Oscillator
	SPI0_TX(RFM_WRITE | REG_OSC1);
	SPI0_TX(RF_OSC1_RCCAL_START);
	temp = 0;
	/*
	while(!temp){
		SPI0_TX(RFM_READ | REG_OSC1); // make read request
		temp = SPI0_RX() & RF_OSC1_RCCAL_DONE; // mask all bits except for RCCAL_DONE
	}
	*/

	// leave RegAfcCtrl as default

	// ignore RegListenX for now

	// set power level to 0 (dBm)
	SPI0_TX((RFM_WRITE | REG_PALEVEL) << 8 | RF_PALEVEL_PA0_ON | RF_PALEVEL_PA1_OFF | RF_PALEVEL_PA2_OFF | RF_PALEVEL_OUTPUTPOWER_10010);

}

void RFM69_TX_Prep(){
	// change operation mode to transmit
	SPI0_TX(RFM_WRITE | REG_OPMODE);
	SPI0_TX(RF_OPMODE_TRANSMITTER);

	// maybe look at configuring SPI to be 8 bits after this poin
}

void RFM69_RX_Prep(){
	// change operation mode to receive
	SPI0_TX(RFM_WRITE | REG_OPMODE);
	SPI0_TX(RF_OPMODE_RECEIVER);
	// maybe look at configuring SPI to be 8 bits after this point
}

void RFM69_TX(uint8_t tx_byte){
	SPI0_TX(RFM_WRITE | REG_FIFO);
	SPI0_TX(tx_byte); // want to write to fifo for TX
}

uint8_t RFM69_RX(){
	SPI0_TX(RFM_READ | REG_FIFO); // want to read from fifo for RX
	return SPI0_RX();
}
