// custom libraries
//#include <SPI0_driver.c>
#include "../SPI0/SPI0_driver.h"
#include "RFM69registers.h"
#include "../GPIO/gpio.h"

// definitions
#define RFM_WRITE 0x80
#define RFM_READ 0x00
#define LISTEN_ABORT_IGNORE 0xE3
#define SEQ_LEN 12
#define networkID 1

// configuration structure and drivers have been modified from https://github.com/LowPowerLab/RFM69/blob/master/RFM69.cpp
const uint8_t CONFIG[][2] = {

		// standbye with sequencer during config
	{REG_OPMODE, RF_OPMODE_SEQUENCER_ON | RF_OPMODE_LISTEN_OFF | RF_OPMODE_STANDBY},

	// packet mode, everything else default
	{REG_DATAMODUL, RF_DATAMODUL_DATAMODE_PACKET | RF_DATAMODUL_MODULATIONTYPE_FSK | RF_DATAMODUL_MODULATIONSHAPING_00},

	// default speed of 55555 bps (testing purposes only)
	{REG_BITRATEMSB, RF_BITRATEMSB_55555},
	{REG_BITRATELSB, RF_BITRATELSB_55555},

	// default fdev of 5kHz
	{REG_FDEVMSB, RF_FDEVMSB_5000},
	{REG_FDEVLSB, RF_FDEVLSB_5000},

	// set carrier to 915 MHz (this is the default value)
	{REG_FRFMSB, RF_FRFMSB_915},
	{REG_FRFMID, RF_FRFMID_915},
	{REG_FRFLSB, RF_FRFLSB_915},

	// power level settings, +5 dBm
	{REG_PALEVEL, RF_PALEVEL_PA0_OFF | RF_PALEVEL_PA1_ON | RF_PALEVEL_PA2_ON | RF_PALEVEL_OUTPUTPOWER_10011},
	//{REG_PALEVEL, RF_PALEVEL_PA0_ON | RF_PALEVEL_PA1_OFF | RF_PALEVEL_PA2_OFF | RF_PALEVEL_OUTPUTPOWER_11111},

	// over current protection, max draw of 95 mA
	{REG_OCP, RF_OCP_ON | RF_OCP_TRIM_95},

	// set to recommended default, reset values are different
	{REG_RXBW, RF_RXBW_DCCFREQ_010 | RF_RXBW_MANT_16 | RF_RXBW_EXP_2},

	// DI0 setup for IRQ, best pin output
	{REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_01},

	// clock out disable as it is not needed
	{REG_DIOMAPPING2, RF_DIOMAPPING2_CLKOUT_OFF},

	// make sure fifos don't get overrun
	{REG_IRQFLAGS2, RF_IRQFLAGS2_FIFOOVERRUN},

	// RSSI Threshold setting
	//{REG_RSSITHRESH, RF_RSSITHRESH_VALUE}, // -value/2 (dBm), -114 dBm
	{REG_RSSITHRESH, 150},

	{REG_SYNCCONFIG, RF_SYNC_ON | RF_SYNC_FIFOFILL_AUTO | RF_SYNC_SIZE_2 | RF_SYNC_TOL_0},

	// pick random sync value
	//{REG_SYNCVALUE1, 0xAA},

	// set network ID, not needed
	//{REG_SYNCVALUE2, networkID},

	// non-fixed length, no DC, CRC on, CRC error throws out data, no address filtering
	{REG_PACKETCONFIG1, RF_PACKET1_FORMAT_VARIABLE | RF_PACKET1_DCFREE_MANCHESTER | RF_PACKET1_CRC_ON | RF_PACKET1_CRCAUTOCLEAR_ON | RF_PACKET1_ADRSFILTERING_OFF},

	// payload may not be longer than largest Barker Code
	{REG_PAYLOADLENGTH, 13},

	// no address filtering
	//{REG_NODEADRS, nodeID},

	// tx when fifo when condition is met
	{REG_FIFOTHRESH, RF_FIFOTHRESH_TXSTART_FIFONOTEMPTY | 13}, //RF_FIFOTHRESH_VALUE, set to 11 bytes in fifo (0xB)

	// no AES, restart after a while, if no receive go into receive mode again
	{REG_PACKETCONFIG2, RF_PACKET2_RXRESTARTDELAY_2BITS | RF_PACKET2_AUTORXRESTART_ON | RF_PACKET2_AES_OFF},

	// apparently helps with fading
	{REG_TESTDAGC, RF_DAGC_IMPROVED_LOWBETA0},

	// give system stopping point for loops when CONFIG[i][0] == 255...
	{255, 0}
};

void RFM69_TX(uint8_t REG, uint8_t tx_byte);

uint8_t RFM69_RX(uint8_t REG);

void RFM69_Init();

void RFM69_SET_MODE(uint8_t mode);

void RFM69_SEND();

void RFM69_RECEIVE();
