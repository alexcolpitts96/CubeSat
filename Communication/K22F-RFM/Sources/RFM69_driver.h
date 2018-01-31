// custom libraries
#include "RFM69registers.h"
#include "SPI0_driver.h"

// definitions
#define RFM_WRITE 0x80
#define RFM_READ 0x00

// configuration structure has been modified from https://github.com/LowPowerLab/RFM69/blob/master/RFM69.cpp
const uint8_t CONFIG[][2] =
{
		// standbye with sequencer during config
	{REG_OPMODE, RF_OPMODE_SEQUENCER_ON | RF_OPMODE_LISTEN_OFF | RF_OPMODE_STANDBY},

	// continuous mode, everything else default
	{REG_DATAMODUL, RF_DATAMODUL_DATAMODE_CONTINUOUS | RF_DATAMODUL_MODULATIONTYPE_FSK | RF_DATAMODUL_MODULATIONSHAPING_00},

	// default speed of 4800 bps (testing purposes only)
	{REG_BITRATEMSB, RF_BITRATEMSB_4800},
	{REG_BITRATELSB, RF_BITRATELSB_4800},

	// default fdev of 5kHz
	{REG_FDEVMSB, RF_FDEVMSB_5000},
	{REG_FDEVLSB, RF_FDEVLSB_5000},

	// set carrier to 915 MHz (this is the default value)
	{REG_FRFMSB, RF_FRFMSB_915},
	{REG_FRFMID, RF_FRFMID_915},
	{REG_FRFLSB, RF_FRFLSB_915},

	// power level setting -18 dBm +  18 dBm (0b10010) = 0 dBm
	{REG_PALEVEL, RF_PALEVEL_PA0_ON | RF_PALEVEL_PA1_OFF | RF_PALEVEL_PA2_OFF | RF_PALEVEL_OUTPUTPOWER_10010},

	// over current protection, max draw of 95 mA
	{REG_OCP, RF_OCP_ON | RF_OCP_TRIM_95},

	// set to recommended default, reset values are different
	{REG_RXBW, RF_RXBW_DCCFREQ_010 | RF_RXBW_MANT_24 | RF_RXBW_EXP_5},

	// ignore DIO registers for now

	// RSSI Threshold setting
	{REG_RSSITHRESH, RF_RSSITHRESH_VALUE}, // -value/2 (dBm)

	// give system stopping point for loops when CONFIG[i][0] == 255...
	{255, 0}
};

void RFM69_Init(){



}

void RFM69_TX_Prep(){
	// change operation mode to transmit
	SPI0_TX(((RFM_WRITE | REG_OPMODE) << 8) | RF_OPMODE_TRANSMITTER);
}

void RFM69_RX_Prep(){
	// change operation mode to receive
	SPI0_TX(((RFM_WRITE | REG_OPMODE) << 8) | RF_OPMODE_RECEIVER);
}

void RFM69_TX(uint8_t tx_byte){
	SPI0_TX(((RFM_WRITE | REG_FIFO) << 8) | tx_byte);
}

uint16_t RFM69_RX(){
	SPI0_TX((RFM_READ | REG_FIFO) << 8); // want to read from fifo for RX
	return SPI0_RX();
}
