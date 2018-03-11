#include "common.h"
#include "flexmem_demo.h"

#include "fsl_device_registers.h"

#define REG_READ(address)             ((unsigned char)(*(volatile unsigned char*)(address)))
#define WRITE8(address,value)         (*(volatile unsigned char*)(address) = (value))
#define READ8(address)                ((unsigned char)(*(volatile unsigned char*)(address)))

unsigned char pCommandArray[10];
unsigned int *ptemp, temp;
unsigned char dataread;

// Flash Initial
void FlashInit(void) {
	while (0 == (FTFL_FSTAT && 0X80)) {
		/* wait till CCIF bit is set */
	}

	/* clear RDCOLERR & ACCERR & FPVIOL flag in flash status register */
	//(FTFx_SSD_FSTAT_RDCOLERR | FTFx_SSD_FSTAT_ACCERR | FTFx_SSD_FSTAT_FPVIOL));
	FTFL_FSTAT = (0x40 | 0x20 | 0x10);
	/* Write Command Code to FCCOB0 */
	FTFL_FCCOB0 = 0x03;
	/* Write address to FCCOB1/2/3 */
	FTFL_FCCOB1 = ((unsigned char) (0x8000FC >> 16));
	FTFL_FCCOB2 = ((unsigned char) ((0x8000FC >> 8) & 0xFF));
	FTFL_FCCOB3 = ((unsigned char) (0x8000FC & 0xFF));

	/* Write Resource Select Code of 0 to FCCOB8 to select IFR. Without this, */
	/* an access error may occur if the register contains data from a previous command. */

	FTFL_FCCOB8 = 0x00;

	/* clear CCIF bit */
	FTFL_FSTAT = 0x80;

	/* check CCIF bit */
	while ((FTFL_FSTAT & 0x80) == 0) {
		/* wait till CCIF bit is set */
	}
}
/* end of file */

// Flash Command Sequence
void FlashCommandSequence(void) {
	/* check CCIF bit of the flash status register */
	while (0 == (FTFL_FSTAT && 0x80)) {
		/* wait till CCIF bit is set */
	}

	/* clear RDCOLERR & ACCERR & FPVIOL flag in flash status register */
	FTFL_FSTAT = (0x40 | 0x20 | 0x10);
	FTFL_FCCOB0 = pCommandArray[0];
	FTFL_FCCOB1 = pCommandArray[1];
	FTFL_FCCOB2 = pCommandArray[2];
	FTFL_FCCOB3 = pCommandArray[3];
	FTFL_FCCOB4 = pCommandArray[4];
	FTFL_FCCOB5 = pCommandArray[5];
	FTFL_FCCOB6 = pCommandArray[6];
	FTFL_FCCOB7 = pCommandArray[7];
	FTFL_FCCOB8 = pCommandArray[8];
	FTFL_FCCOB9 = pCommandArray[9];
	/* clear CCIF bit */
	FTFL_FSTAT = 0x80;
}

//Flash Partition
void DEFlashPartition(unsigned char EEEDataSizeCode,
		unsigned char DEPartitionCode) {
	/* preparing passing parameter to program partition command */
	pCommandArray[0] = 0x80;
	pCommandArray[4] = EEEDataSizeCode;
	pCommandArray[5] = DEPartitionCode;

	/* calling flash command sequence function to execute the command */
	FlashCommandSequence();

}

// Flash Erase All Block
void FlashEraseAllBlock(void) {
	/* preparing passing parameter to erase all flash blocks */
	/* 1st element for the FCCOB register */
	pCommandArray[0] = 0x44;

	/* calling flash command sequence function to execute the command */
	FlashCommandSequence();

	/* Enter Debug state if enabled */
}

//EEPROM Write
void EEEWrite(unsigned long destination, unsigned long size,
		unsigned long source) {
	/* Check if EEE is enabled */
	if (REG_READ(FTFL_FCNFG & 0x01)) {
		while (size > 0) {
			while (0 == (FTFL_FCNFG & 0x01)) {
				/* wait till EEERDY bit is set */
			}
			WRITE8(destination, READ8(source));

			/* Check for protection violation error */
			if (REG_READ(FTFL_FSTAT & 0x10)) {
			}

			/* update destination address for next iteration */
			destination += 1;
			/* update size for next iteration */
			size -= 1;
			/* increment the source adress by 1 */
			source += 1;
		}
	}
}

void FlashProgramLongword(unsigned int destination, unsigned int size,
		unsigned int source) {
	pCommandArray[0] = 0x06;
	pCommandArray[1] = (unsigned char) (destination >> 16);
	pCommandArray[2] = (unsigned char) ((destination >> 8) & 0xFF);
	pCommandArray[3] = (unsigned char) (destination & 0xFF);
	pCommandArray[4] = READ8(source + 3);
	pCommandArray[5] = READ8(source + 2);
	pCommandArray[6] = READ8(source + 1);
	pCommandArray[7] = READ8(source);
	FlashCommandSequence();
}
/*********************************************************************
 *
 *  Function Name    : main
 *  Description      : Main function
 *
 *  Arguments        : void
 *  Return Value     : UNIT32
 *
 **********************************************************************/

void main(void) {

	unsigned long source;
	unsigned long address;
	unsigned long size;
	unsigned char buffer[0x20] = { 0x33, 0x44, 0x55 };
	unsigned char dataread[0x20] = { 0x00, 0x00, 0x00 };
	unsigned char i = 0;
	unsigned int unsecure_key = 0xFFFFFFFE;

	/**************************************************************************
	 *                               FlashInit()                               *
	 ***************************************************************************/
	FlashInit();

	/**************************************************************************
	 *                          FlashEraseAllBlock()                           *
	 ***************************************************************************/
	FlashEraseAllBlock();
	FlashProgramLongword(0x40C, 4, (unsigned int) (&unsecure_key));
	/**************************************************************************
	 *                            DEFlashPartition()                           *
	 ***************************************************************************/
	DEFlashPartition(0x03, 0x05);
	/* Call FlashInit again to get the new Flash configuration */
	FlashInit();
	/**************************************************************************
	 *                               EEEWrite()                                *
	 ***************************************************************************/
	//destination = flashSSDConfig.EERAMBlockBase;
	address = 0x14000000;
	size = 0x03;
	source = (unsigned int) buffer;
	EEEWrite(address, size, source);
	dataread[0] = *((unsigned char *) 0x14000000);
	while (1)
		;
}
