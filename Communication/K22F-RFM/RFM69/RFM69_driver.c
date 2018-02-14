// custom libraries
//#include <SPI0_driver.c>
#include "../SPI0/SPI0_driver.h"
#include "RFM69registers.h"
#include "../GPIO/gpio.h"

// definitions
#define RFM_WRITE 0x80
#define RFM_READ 0x00
#define LISTEN_ABORT_IGNORE 0xE3
#define MAX_STRING_LENGTH 26

// configuration structure and drivers have been modified from https://github.com/LowPowerLab/RFM69/blob/master/RFM69.cpp
const uint8_t CONFIG[][2] = {

		// standby mode
	{REG_OPMODE, RF_OPMODE_STANDBY},

	// packet mode, everything else default
	{REG_DATAMODUL, RF_DATAMODUL_DATAMODE_PACKET | RF_DATAMODUL_MODULATIONTYPE_FSK | RF_DATAMODUL_MODULATIONSHAPING_00},

	// set speed to 4800 bps (testing purposes only)
	{REG_BITRATEMSB, RF_BITRATEMSB_4800},
	{REG_BITRATELSB, RF_BITRATELSB_4800},

	// fdev of 10kHz
	{REG_FDEVMSB, RF_FDEVMSB_10000},
	{REG_FDEVLSB, RF_FDEVLSB_10000},

	// set carrier to 915 MHz (this is the default value)
	{REG_FRFMSB, RF_FRFMSB_915},
	{REG_FRFMID, RF_FRFMID_915},
	{REG_FRFLSB, RF_FRFLSB_915},

	// start calibration
	{REG_OSC1, RF_OSC1_RCCAL_START},

	// power level settings
	//{REG_PALEVEL, RF_PALEVEL_PA0_OFF | RF_PALEVEL_PA1_ON | RF_PALEVEL_PA2_ON | RF_PALEVEL_OUTPUTPOWER_11111},
	{REG_PALEVEL, RF_PALEVEL_PA0_ON | RF_PALEVEL_PA1_OFF | RF_PALEVEL_PA2_OFF | RF_PALEVEL_OUTPUTPOWER_11111},

	// over current protection, max draw of 95 mA
	{REG_OCP, RF_OCP_ON | RF_OCP_TRIM_95},

	// set to 200 ohm impedance since it isn't matched
	{REG_LNA, RF_LNA_ZIN_200},

	// set to recommended default, reset values are different
	{REG_RXBW, RF_RXBW_DCCFREQ_010 | RF_RXBW_MANT_16 | RF_RXBW_EXP_2},

	// DI0 setup for IRQ, best pin output
	{REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_01},

	// clock out disable as it is not needed
	{REG_DIOMAPPING2, RF_DIOMAPPING2_CLKOUT_OFF},

	// RSSI Threshold setting, set to -100 dBm (-value/2 in dBm)
	{REG_RSSITHRESH, 200},

	// preamble bytes, standard values
	{REG_PREAMBLEMSB, 0},
	{REG_PREAMBLELSB, 3},

	// may need sync_size set to lower number
	{REG_SYNCCONFIG, RF_SYNC_ON | RF_SYNC_FIFOFILL_AUTO | RF_SYNC_SIZE_4 | RF_SYNC_TOL_0},

	// pick random sync value
	{REG_SYNCVALUE1, 0x2D},
	{REG_SYNCVALUE2, 0x3C},
	{REG_SYNCVALUE3, 0xD2},
	{REG_SYNCVALUE4, 0xC3},

	// non-fixed length, no DC, CRC on, CRC error throws out data, no address filtering, RF_PACKET1_DCFREE_MANCHESTER
	//{REG_PACKETCONFIG1, RF_PACKET1_FORMAT_VARIABLE | RF_PACKET1_DCFREE_OFF | RF_PACKET1_CRC_ON | RF_PACKET1_CRCAUTOCLEAR_ON | RF_PACKET1_ADRSFILTERING_OFF},
	{REG_PACKETCONFIG1, RF_PACKET1_FORMAT_FIXED | RF_PACKET1_DCFREE_MANCHESTER | RF_PACKET1_CRC_ON | RF_PACKET1_CRCAUTOCLEAR_ON | RF_PACKET1_ADRSFILTERING_OFF},

	// payload may not be longer than buffer
	{REG_PAYLOADLENGTH, MAX_STRING_LENGTH},

	// tx fifo when condition is met
	{REG_FIFOTHRESH, RF_FIFOTHRESH_TXSTART_FIFOTHRESH | (MAX_STRING_LENGTH-1)}, // as soon as full packet is in the system will transmit
	//{REG_FIFOTHRESH, RF_FIFOTHRESH_TXSTART_FIFONOTEMPTY | (MAX_STRING_LENGTH-1)},

	// no AES, restart after a while, if no receive go into receive mode again
	{REG_PACKETCONFIG2, RF_PACKET2_RXRESTARTDELAY_2BITS | RF_PACKET2_AUTORXRESTART_ON | RF_PACKET2_AES_OFF},
	//{REG_PACKETCONFIG2, RF_PACKET2_RXRESTARTDELAY_2BITS | RF_PACKET2_AUTORXRESTART_OFF | RF_PACKET2_AES_OFF},

	// apparently helps with fading
	{REG_TESTDAGC, RF_DAGC_IMPROVED_LOWBETA0},

	// give system stopping point for loops when CONFIG[i][0] == 255...
	{255, 0}
};

void RFM69_TX(uint8_t REG, uint8_t tx_byte){
	SPI0_TX(((RFM_WRITE | REG) << 8) | tx_byte); // write to REG
}

uint8_t RFM69_RX(uint8_t REG){
	SPI0_TX((RFM_READ | REG) << 8); // want to read from REG
	return SPI0_RX() & 0xFF;
}

void RFM69_Init(){
	uint8_t i;

	SPI0_Prep(); // ensure SPI0 is ready for tx/rx

	// sync to the module to ensure reading and writing is working properly
	// no need to wait for timeout, this either works or the whole system dies

	// send alternating 1s and 0s
	while(RFM69_RX(REG_SYNCVALUE1) != 0xAA){
		RFM69_TX(REG_SYNCVALUE1, 0xAA);
	}

	// send alternating 0s and 1s
	while(RFM69_RX(REG_SYNCVALUE1) != 0x55){
		RFM69_TX(REG_SYNCVALUE1, 0x55);
	}

	// write the config structure to the device
	for(i = 0; CONFIG[i][0] != 255; i++){
		RFM69_TX(CONFIG[i][0], CONFIG[i][1]); // column 1 is reg, column 2 is data
	}

	// system was set into standby during configuration
	// make sure calibration of OSC1 completed
	while(!(RFM69_RX(REG_OSC1) & RF_OSC1_RCCAL_DONE));

	// setup GPIO for flag receiving
	RFM69_DIO0_Init();
}

// clear the FIFO as needed
void RFM69_CLEAR_FIFO(){
	RFM69_TX(REG_IRQFLAGS2, RF_IRQFLAGS2_FIFOOVERRUN); // clear flag and reset FIFO
}

// pass in mode to have the system set into it
void RFM69_SET_MODE(uint8_t mode){
	uint8_t read;

	// read in current REG_OPMODE value, ignore ListenAbort field
	read = RFM69_RX(REG_OPMODE) & LISTEN_ABORT_IGNORE;

	// check current mode and change to receive if needed
	if(read != mode){
		RFM69_TX(REG_OPMODE, mode);
	}

	// wait for RFM to be ready
	while((RFM69_RX(REG_IRQFLAGS1) & RF_IRQFLAGS1_MODEREADY) == 0);
}

// send buffer, will be seq_length bytes (11 bytes), see SEQ_LEN definition
void RFM69_SEND(uint8_t *buffer){
	uint8_t i;

	// set RFM to standby to load the FIFO
	RFM69_SET_MODE(RF_OPMODE_STANDBY);

	// DIO0 will be packet sent when put into tx mode
	RFM69_TX(REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_00);

	// clear FIFO to ensure no data is there
	RFM69_CLEAR_FIFO();

	// read the buffer into the FIFO
	for(i = 0; i < MAX_STRING_LENGTH; i++){
		RFM69_TX(REG_FIFO, buffer[i]);
	}

	// set to tx mode, RFM does the work
	RFM69_SET_MODE(RF_OPMODE_TRANSMITTER);

	// wait for packet to be sent by checking GPIO or flag
	//while(!RFM69_DIO0_Read());
	while(!(RFM69_RX(REG_IRQFLAGS2) & RF_IRQFLAGS2_PACKETSENT));

	// return to standby when done
	RFM69_SET_MODE(RF_OPMODE_STANDBY);
}

// check if payloadready flag has been set, return 1 if set
uint8_t RFM69_PL_RD(){

	if(RFM69_RX(REG_IRQFLAGS2) & RF_IRQFLAGS2_PAYLOADREADY){
		return 1;
	}
	else{
		return 0;
	}
}

// incomplete function
void RFM69_RECEIVE(uint8_t *buffer){ // look at page 45, 5.2.2.3
	uint8_t read, temp, i, payload_len;

	// may not be needed
	RFM69_SET_MODE(RF_OPMODE_STANDBY);

	//read = RFM69_DIO0_Read();

	// set DIO0 to payloadready in receive mode
	RFM69_TX(REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_01);

	// set mode to receiver
	RFM69_SET_MODE(RF_OPMODE_RECEIVER);

	// clear fifo and reset flags
	RFM69_CLEAR_FIFO();

	// wait for payload ready to go high
	//while(!RFM69_DIO0_Read());
	while(!RFM69_PL_RD());

	// check CRC
	while(!(RFM69_RX(REG_IRQFLAGS2) & RF_AUTOMODES_ENTER_CRCOK));

	// set to standby once a package has been received to save power
	RFM69_SET_MODE(RF_OPMODE_STANDBY);

	i = 0;
	//for(i = 0; i < MAX_STRING_LENGTH; i++){
	while(i < MAX_STRING_LENGTH && !(RFM69_RX(REG_IRQFLAGS2) & RF_AUTOMODES_ENTER_FIFONOTEMPTY)){
	//while(RFM69_RX(REG_IRQFLAGS2) & RF_IRQFLAGS2_PAYLOADREADY){
		buffer[i] = RFM69_RX(REG_FIFO);
		i++;
	}

	return;
}


