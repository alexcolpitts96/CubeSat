/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// standard libraries
#include "fsl_device_registers.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"

// custom libraries
#include "../RFM69/RFM69_driver.h"
#include "../UART1/UART1_driver.h"
#include "../FTM/FTM_driver.h"
#include "../UART0/UART0_driver.h"
#include "../SPI0/SPI0_driver.h"
#include "../GPIO/gpio.h" // included in RFM69 driver
#include "../Comms/Comms.h"
#include "../Camera/camera.h"
#include "../I2C/i2c.h"

/* size of array to copy__Launch_Command function to.*/
/* It should be at least equal to actual size of __Launch_Command func */
/* User can change this value based on RAM size availability and actual size of __Launch_Command function */
#define LAUNCH_CMD_SIZE         0x100

/* Size of function used for callback.  Change this depending on the size of your function */
#define CALLBACK_SIZE           0x80

#define BUFFER_SIZE_BYTE        0x80

#define FTFx_REG_BASE           0x40020000
#define P_FLASH_BASE            0x00000000

/* Program Flash block information */
#define P_FLASH_SIZE            (FSL_FEATURE_FLASH_PFLASH_BLOCK_SIZE * FSL_FEATURE_FLASH_PFLASH_BLOCK_COUNT)
#define P_BLOCK_NUM             FSL_FEATURE_FLASH_PFLASH_BLOCK_COUNT
#define P_SECTOR_SIZE           FSL_FEATURE_FLASH_PFLASH_BLOCK_SECTOR_SIZE
/* Data Flash block information */
#define FLEXNVM_BASE            FSL_FEATURE_FLASH_FLEX_NVM_START_ADDRESS
#define FLEXNVM_SECTOR_SIZE     FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SECTOR_SIZE
#define FLEXNVM_BLOCK_SIZE      FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SIZE
#define FLEXNVM_BLOCK_NUM       FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_COUNT

/* Flex Ram block information */
#define EERAM_BASE              FSL_FEATURE_FLASH_FLEX_RAM_START_ADDRESS
#define EERAM_SIZE              FSL_FEATURE_FLASH_FLEX_RAM_SIZE

/* Destination to program security key back to flash location */
#if (FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE == 8)
#define SECURITY_LOCATION         0x408
#else /* FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE == 4 */
#define SECURITY_LOCATION         0x40C
#endif

#define BACKDOOR_KEY_LOCATION     0x400

/* Program flash IFR map*/
#if (FSL_FEATURE_FLASH_IS_FTFE == 1)
#define PFLASH_IFR                0x3C0
#else /* FSL_FEATURE_FLASH_IS_FTFL == 1 or FSL_FEATURE_FLASH_IS_FTFA = =1 */
#define PFLASH_IFR                0xC0
#endif

#if (FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP == 1)
#if (FSL_FEATURE_FLASH_IS_FTFE == 1)
#define SWAP_STATUS_BIT (REG_READ(FTFx_REG_BASE + FTFx_SSD_FCNFG_OFFSET) & FTFE_FCNFG_SWAP_MASK)
#endif
#if (FSL_FEATURE_FLASH_IS_FTFL == 1)
#define SWAP_STATUS_BIT (REG_READ(FTFx_REG_BASE + FTFx_SSD_FCNFG_OFFSET) & FTFL_FCNFG_SWAP_MASK)
#endif
#if (FSL_FEATURE_FLASH_IS_FTFA == 1)
#define SWAP_STATUS_BIT (REG_READ(FTFx_REG_BASE + FTFx_SSD_FCNFG_OFFSET) & FTFA_FCNFG_SWAP_MASK)
#endif
#endif

/* Has flash cache control in MCM module */
#if (FSL_FEATURE_FLASH_HAS_MCM_FLASH_CACHE_CONTROLS == 1)
#define CACHE_DISABLE             MCM_BWR_PLACR_DFCS(MCM_BASE_PTR, 1);
/* Has flash cache control in FMC module */
#elif (FSL_FEATURE_FLASH_HAS_FMC_FLASH_CACHE_CONTROLS == 1)
#if defined(FMC_PFB1CR) && defined(FMC_PFB1CR_B1SEBE_MASK)
#define CACHE_DISABLE     FMC_PFB0CR &= ~(FMC_PFB0CR_B0SEBE_MASK | FMC_PFB0CR_B0IPE_MASK | FMC_PFB0CR_B0DPE_MASK | FMC_PFB0CR_B0ICE_MASK | FMC_PFB0CR_B0DCE_MASK);\
                                  FMC_PFB1CR &= ~(FMC_PFB1CR_B1SEBE_MASK | FMC_PFB1CR_B1IPE_MASK | FMC_PFB1CR_B1DPE_MASK | FMC_PFB1CR_B1ICE_MASK | FMC_PFB1CR_B1DCE_MASK);
#elif defined(FMC_PFB23CR)
#define CACHE_DISABLE     FMC_PFB01CR &= ~(FMC_PFB01CR_B0IPE_MASK | FMC_PFB01CR_B0DPE_MASK | FMC_PFB01CR_B0ICE_MASK | FMC_PFB01CR_B0DCE_MASK);\
                                  FMC_PFB23CR &= ~(FMC_PFB23CR_B1IPE_MASK | FMC_PFB23CR_B1DPE_MASK | FMC_PFB23CR_B1ICE_MASK | FMC_PFB23CR_B1DCE_MASK);
#else
#define CACHE_DISABLE     FMC_PFB0CR &= ~(FMC_PFB0CR_B0SEBE_MASK | FMC_PFB0CR_B0IPE_MASK | FMC_PFB0CR_B0DPE_MASK | FMC_PFB0CR_B0ICE_MASK | FMC_PFB0CR_B0DCE_MASK);
#endif
#else
/* No cache in the device */
#define CACHE_DISABLE
#endif

/***************************************************************************************/
/***************************************************************************************/

/* Other defines */
#define DEBUGENABLE               0x00

#define READ_NORMAL_MARGIN        0x00
#define READ_USER_MARGIN          0x01
#define READ_FACTORY_MARGIN       0x02

#define ONE_KB                  1024                        //0x400:  10 zeros
#define TWO_KB                  (2*ONE_KB)
#define THREE_KB                (3*ONE_KB)
#define FOUR_KB                 (4*ONE_KB)
#define FIVE_KB                 (5*ONE_KB)
#define SIX_KB                  (6*ONE_KB)
#define SEVEN_KB                (7*ONE_KB)
#define EIGHT_KB                (8*ONE_KB)
#define NINE_KB                 (9*ONE_KB)
#define TEN_KB                  (10*ONE_KB)
#define ONE_MB                  (ONE_KB*ONE_KB)             //0x100000:     20 zeros
#define ONE_GB                  (ONE_KB*ONE_KB*ONE_KB)      //0x40000000:   30 zeros

/********************************************************/
/*      Global Variables                                */
/********************************************************/
//uint8_t DataArray[PGM_SIZE_BYTE];
uint8_t program_buffer[BUFFER_SIZE_BYTE];
uint32_t gCallBackCnt; /* global counter in callback(). */
//pFLASHCOMMANDSEQUENCE g_FlashLaunchCommand = (pFLASHCOMMANDSEQUENCE) 0xFFFFFFFF;

/* array to copy __Launch_Command func to RAM */
uint16_t __ram_func[LAUNCH_CMD_SIZE / 2];
uint16_t __ram_for_callback[CALLBACK_SIZE / 2]; /* length of this array depends on total size of the functions need to be copied to RAM*/

//FLASH_SSD_CONFIG flashSSDConfig = { FTFx_REG_BASE, /* FTFx control register base */
//P_FLASH_BASE, ///* base address of PFlash block */
//P_FLASH_SIZE, ///* size of PFlash block */
//FLEXNVM_BASE, ///* base address of DFlash block */
//0, /* size of DFlash block */
//EERAM_BASE, ///* base address of EERAM block */
//0, ///* size of EEE block */
//DEBUGENABLE, ///* background debug mode enable bit */
//NULL_CALLBACK ///* pointer to callback function */
//};
//*/

/*********************************************************************
 *
 *  Function Name    : ErrorTrap
 *  Description      : Gets called when an error occurs.
 *  Arguments        : uint32_t
 *  Return Value     :
 *
 *********************************************************************/
void ErrorTrap(uint32_t ret) {
	while (1) {
		;
	}
}

/*********************************************************************
 *
 *  Function Name    : callback
 *  Description      : callback function for flash operations
 *  Arguments        : none
 *  Return Value     :
 *
 *********************************************************************/
void callback(void) {
	/* Should not use global variable for functions which need to be
	 * relocated such as callback function. If used, some compiler
	 * such as KEIL will be failed in all flash functions and
	 * the bus error will be triggered.
	 */
}

uint32_t ret; /* Return code from each SSD function */
uint32_t destination; /* Address of the target location */
uint32_t size;
uint32_t end;
uint8_t securityStatus; /* Return protection status */
uint16_t number; /* Number of longword or phrase to be program or verify*/
uint32_t *p_data;
uint32_t margin_read_level; /* 0=normal, 1=user - margin read for reading 1's */

#define MAX_IMAGE_SIZE 0x5FFFF

void master_init() {
	UART0_Init();
	UART1_putty_init();
	SPI0_Init(16);
	RFM69_DIO0_Init();
	RFM69_Init(); // must always be after the SPI interface has been enabled
	FTM0_init();
	FTM1_init();
	init_I2C();
	SPI1_Init(16);
	camera_init();
}

int main() {

	/*
	 // set program buffer to only have 10 bytes of interest
	 for (int i = 0; i < BUFFER_SIZE_BYTE; i++) {
	 if (i < 10) {
	 program_buffer[i] = i;
	 } else {
	 program_buffer[i] = 0;
	 }
	 }


	 // test flash programming and reading back
	 destination = flashSSDConfig.PFlashBase
	 + (flashSSDConfig.PFlashSize - 6 * FTFx_PSECTOR_SIZE);
	 size = BUFFER_SIZE_BYTE;

	 // erase sector
	 ret = FlashEraseSector(&flashSSDConfig, destination, size,
	 g_FlashLaunchCommand);

	 // program memory
	 ret = FlashProgram(&flashSSDConfig, destination, size, program_buffer,
	 g_FlashLaunchCommand);

	 uint8_t return_buffer[BUFFER_SIZE_BYTE];
	 ret = FlashReadResource(&flashSSDConfig, destination, program_buffer, 1,
	 g_FlashLaunchCommand);

	 memset(return_buffer, 0, sizeof(uint8_t) * BUFFER_SIZE_BYTE);
	 uint8_t *flash_pointer = destination;
	 for (int i = 0; i < 10; i++) {
	 return_buffer[i] = flash_pointer[i];
	 }
	 //*/

	int mode_select = 8; // 8 is satellite, 9 is ground station
	//uint8_t *buffer = (uint8_t *) calloc(PACKET_SIZE, sizeof(uint8_t));
	//uint8_t *camera = (uint8_t *) calloc(PACKET_SIZE, sizeof(uint8_t));

	uint8_t buffer_arr[PACKET_SIZE];
	uint8_t camera_arr[PACKET_SIZE];
	uint8_t *buffer = buffer_arr; // may need to be the address
	uint8_t *camera = camera_arr;
	uint8_t image[7500];

	master_init();

	while (mode_select == 8) {
		int image_length;

		capture();
		image_length = fifo_len();

		///*
		// read the image into the array
		for (int i = 0; i < image_length; i++) {
			image[i] = cam_reg_read(0x3D);

			putty_putchar(image[i]);

			// this had no errors
			//image[i] = '0' + (i%10);
		}

		for (int i = 0; i < image_length; i++) {
			putty_putchar(image[i]);
		}
		//*/

		// transmit size of the image and wait for the start command
		imageSize(buffer, image_length);

		// transmit packets until stop command received
		while (transmitPacket(buffer, camera, image))
			;

		// clear the camera memory
		flush_fifo();
	}

// packetRequest test - ground station
	while (mode_select == 9) {

		uint32_t image_bytes = txStart(buffer);
		uint32_t packet_number = (uint32_t) ceil(
				(float) image_bytes / (float) PACKET_SIZE);

		// retrieve all of the packets
		for (int i = 0; i < packet_number; i++) {
			packetRequest(buffer, i);
		}

		// send the stop command once image received
		memset(buffer, '\0', sizeof(uint8_t) * PACKET_SIZE);
		memcpy((uint8_t *) buffer, &stop_command, sizeof(stop_command));
		RFM69_SEND(buffer);
		RFM69_SEND(buffer);
		RFM69_SEND(buffer);
		RFM69_SEND(buffer);
		RFM69_SEND(buffer);
	}

	return 0;
}
