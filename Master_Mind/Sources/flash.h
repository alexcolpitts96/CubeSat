#ifndef FLASH_H
#define FLASH_H

#include "fsl_device_registers.h"

// ported from https://community.nxp.com/thread/307463 example

#define REG_READ(address)             ((unsigned char)(*(volatile unsigned char*)(address)))
#define WRITE8(address,value)         (*(volatile unsigned char*)(address) = (value))
#define READ8(address)                ((unsigned char)(*(volatile unsigned char*)(address)))

unsigned char pCommandArray[10];
unsigned int *ptemp, temp;
unsigned char dataread;

// flash initialization
void FlashInit(void) {
	// logic error, needed to be bitwise &
	// wait until flash command is completed
	while (0 == (FTFA_FSTAT & FTFA_FSTAT_CCIF_MASK));

	// clear flags for violation, access and collision
	FTFA_FSTAT = (FTFA_FSTAT_RDCOLERR_MASK | FTFA_FSTAT_ACCERR_MASK | FTFA_FSTAT_FPVIOL_MASK);

	// SEE PAGE 642 FOR DETAILS ON FTFA_FCC0Bn

	/* Write Command Code to FCCOB0 */
	// this is the command register
	FTFA_FCCOB0 = 0x03;

	/* Write address to FCCOB1/2/3 */
	// 1-3 are flash address, flash memory located in ch. 4
	// need to change these addresses
	FTFA_FCCOB1 = ((unsigned char) (0x8000FC >> 16));
	FTFA_FCCOB2 = ((unsigned char) ((0x8000FC >> 8) & 0xFF));
	FTFA_FCCOB3 = ((unsigned char) (0x8000FC & 0xFF));

	/* Write Resource Select Code of 0 to FCCOB8 to select IFR. Without this, */
	/* an access error may occur if the register contains data from a previous command. */

	FTFA_FCCOB8 = 0x00;

	/* clear CCIF bit */
	FTFA_FSTAT = FTFA_FSTAT_CCIF_MASK;

	// wait until flash command is completed
	while (0 == (FTFA_FSTAT & FTFA_FSTAT_CCIF_MASK));
}
/* end of file */

#endif FLASH_H
