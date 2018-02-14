# Communication Overview

The CubeSat communication system must be low power and able to transmit at least 400 km.

# VAF: Adaptive Spread Spectrum
The location of the satellite will directly affect the received power at the ground station. This will likely be the limiting factor in the system. The ground station can transmit 4W EIRP which will easily be able to be picked up by the satellite. On the contrary, the satellite will only be able to transmit 20 dBm, 21 is the antenna directivity is accounted for. 

In order to improve the data rate the system may use adaptive spread spectrum to improve the data rate. The system will likely be using Barker Codes to improve the received signal at the ground station using post processing. The data rate will be directly reduced by a factor of the data rate over the sequence lenght. Reducing the sequence length by from 13 to 11 will improve the effective data rate from 23 kbps to 27 kbps allowing the images to be transmitted faster. 

Based on the received signal strength estimates for the closest part of the satellite orbit, the removal of the sequence all together may be a possibility thus increasing the data rate to the maximum of 300 kbps.

# RFM69 Config Details

0x01 - REG_OPMODE - 		set to standby mode
0x02 - REG_DATAMODUL - 		packet mode, FSK, no shaping
0x03 - REG_BITRATEMSB - 	set to 4.8 kbps
0x04 - REG_BITRATELSB - 	set to 4.8 kbps
0x05 - REG_FDEVMSB - 		set to 10 kHz 
0x06 - REG_FDEVLSB - 		set to 10 kHz 
0x07 - REG_FRFMSB - 		set to 915 MHz
0x08 - REG_FRFMID - 		set to 915 MHz
0x09 - REG_FRFLSB - 		set to 915 MHz
0x0A - REG_OSC1 - 		start calibration
0x0B - ----------------------------------------------------------
0x0C - unused
0x0D - REG_LISTEN1 - 		default values
0x0E - REG_LISTEN2 - 		default values
0x0F - REG_LISTEN3 - 		default values
0x10 - REG_VERSION - 		no use
0x11 - REG_PALEVEL - 		set power level
0x12 - REG_PARAMP - 		default ramp time
0x13 - REG_OCP - 		set to 95 mA
0x14 - ----------------------------------------------------------
0x15 - ----------------------------------------------------------
0x16 - ----------------------------------------------------------
0x17 - ----------------------------------------------------------
0x18 - REG_LNA - 		set to 200 ohm since the antenna isn't matched ***************
0x19 - REG_RXBW - 		set to suggested value
0x1A - No changes 
0x1B - OOK related - 		no changes
0x1C - OOK related - 		no changes
0x1D - OOK related - 		no changes
0x1E - AFC related - 		no changes
0x1F - AFC related - 		no changes
0x20 - AFC related - 		no changes
0x21 - FEI related - 		no changes
0x22 - FEI related - 		no changes
0x23 - RSSI config - 		no changes
0x24 - RSSI value - 		no changes
0x25 - REG_DIOMAPPING1 - 	set DIO for DIO0 to config 01
0x26 - REG_DIOMAPPING2 - 	turn off external clock
0x27 - REG_IRQFLAGS1 - 		not configured, for flags
0x28 - REG_IRQFLAGS2 - 		not configured, for flags
0x29 - REG_RSSITHRESH - 	RSSI threshold set to -100 dBm
0x2A - REG_RXTIMEOUT1 - 	set timeout interupt after a while if nothing received 	*************************** may be useful
0x2B - REG_RXTIMEOUT2 - 	set timeout interupt after a while if RSSI doesn't pass	*************************** may be useful
0x2C - REG_PREAMBLEMSB - 	preamble bytes - set to 3 bytes of preamble
0x2D - REG_PREAMBLELSB - 	preamble bytes - set to 3 bytes of preamble
0x2E - REG_SYNCCONFIG - 	sync word is on, 4 sync words, no errors tolerated
0x2F - REG_SYNCVALUE1 - 	set based on number of sync words in 0x2E
to 
0x36 - REG_SYNCVALUE8
0x37 - REG_PACKETCONFIG1 - 	fixed length, manchester encoding, CRC on, autoclear is on, no address filtering
0x38 - REG_PAYLOADLENGTH - 	payload is the maximum packet data size, MAX STRING LENGTH
0x39 - REG_NODEADRS - 		unused since no address filtering
0x3A - REG_BROADCASTADRS - 	unused since no address filtering
0x3B - REG_AUTOMODES - 		auto modes unused
0x3C - REG_FIFOTHRESH - 	tx on fifo threshold exceeded or fifo not empty, fifo threshold will be MAX STRING LENGTH-1 to make sure all bytes are sent
0x3D - REG_PACKETCONFIG2 - 	ignore packet delay, auto restart rx on, try off, no AES *************************** may be useful, try rx restart



