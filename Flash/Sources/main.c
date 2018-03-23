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

// Standard C Included Files
#include <string.h>
#include <stdio.h>
#include "stdlib.h"
#include "string.h"
#include "math.h"

// SDK Included Files
#include "board.h"
#include "main.h"
#include "fsl_debug_console.h"
#include "fsl_device_registers.h"
#include "SSD_FTFx.h"
#include "flash_demo.h"
#include "fsl_interrupt_manager.h"

// custom libraries
#include "../RFM69/RFM69_driver.h"
#include "../UART1/UART1_driver.h"
#include "../FTM/FTM_driver.h"
#include "../UART0/UART0_driver.h"
#include "../SPI0/SPI0_driver.h"
#include "../SPI/spi.h"
#include "../GPIO/gpio.h" // included in RFM69 driver
#include "../Comms/Comms.h"
#include "../Camera/camera.h"
#include "../I2C/i2c.h"

#define FLASH_BLOCKS 147

//uint8_t DataArray[PGM_SIZE_BYTE];
//uint8_t program_buffer[BUFFER_SIZE_BYTE];
uint8_t program_buffer[FTFx_PSECTOR_SIZE]; // size of pflash sector

//uint8_t return_buffer[BUFFER_SIZE_BYTE];
uint32_t gCallBackCnt; /* global counter in callback(). */
pFLASHCOMMANDSEQUENCE g_FlashLaunchCommand = (pFLASHCOMMANDSEQUENCE) 0xFFFFFFFF;

uint16_t ramFunc[LAUNCH_CMD_SIZE / 2];
/* array to copy __Launch_Command func to RAM */
uint16_t __ram_func[LAUNCH_CMD_SIZE / 2];
uint16_t __ram_for_callback[CALLBACK_SIZE / 2]; /* length of this array depends on total size of the functions need to be copied to RAM*/

FLASH_SSD_CONFIG flashSSDConfig = { FTFx_REG_BASE, /* FTFx control register base */
P_FLASH_BASE, /* base address of PFlash block */
P_FLASH_SIZE, /* size of PFlash block */
FLEXNVM_BASE, /* base address of DFlash block */
0, /* size of DFlash block */
EERAM_BASE, /* base address of EERAM block */
0, /* size of EEE block */
DEBUGENABLE, /* background debug mode enable bit */
NULL_CALLBACK /* pointer to callback function */
};

// get stuck if an error occurs
void error_trap(uint32_t ret) {
	if (ret != FTFx_OK) {
		while (1)
			;
	}
}

// disable all GPIO modules
void disable_modules() {
	// turn off port clocks, NOT and then masks to the register
	SIM_SCGC5 &= ~SIM_SCGC5_PORTA_MASK;
	SIM_SCGC5 &= ~SIM_SCGC5_PORTB_MASK;
	SIM_SCGC5 &= ~SIM_SCGC5_PORTC_MASK;
	SIM_SCGC5 &= ~SIM_SCGC5_PORTD_MASK;
	SIM_SCGC5 &= ~SIM_SCGC5_PORTE_MASK;

	// turn off module clocks, NOT and then mask to the register
	SIM_SCGC4 &= ~SIM_SCGC4_I2C0_MASK;
	SIM_SCGC6 &= ~SIM_SCGC6_SPI1_MASK;
	SIM_SCGC6 &= ~SIM_SCGC6_SPI0_MASK;
	SIM_SCGC6 &= ~SIM_SCGC6_FTM0_MASK;
	SIM_SCGC6 &= ~SIM_SCGC6_FTM1_MASK;
}

void master_init() {
	//UART0_Init();
	//UART1_putty_init();
	SPI0_Init(16);
	RFM69_DIO0_Init();
	RFM69_Init(); // must always be after the SPI interface has been enabled
	FTM0_init();
	FTM1_init();
	init_I2C();
	SPI1_Init(16);
	camera_init();
}

// NOTE: for FRDMK22F There is 6 blocks of 2048 bytes to use
int main(void) {
	uint8_t *flash_pointer;
	uint32_t ret; /* Return code from each SSD function */
	uint32_t destination; /* Address of the target location */
	uint32_t size;
	//uint32_t end;
	//uint8_t securityStatus; /* Return protection status */
	//uint16_t number; /* Number of longword or phrase to be program or verify*/
	//uint32_t *p_data;
	uint32_t margin_read_level; /* 0=normal, 1=user - margin read for reading 1's */
	uint32_t FailAddr;
	uint32_t image_length;
	uint8_t buffer_arr[PACKET_SIZE];
	uint8_t *buffer = buffer_arr; // may need to be the address

	gCallBackCnt = 0;
	int mode_select = 8; // S is 8, GS is 9
	//uint8_t *temp = test_image;

	//CACHE_DISABLE

	// init board hardware
	hardware_init();
	master_init();

	// always loop through the following
	while (1) {

		// capture information from satellite and store in flash
		while (mode_select == 8) {

			// init the flash memory
			ret = FlashInit(&flashSSDConfig);
			error_trap(ret);

			// move flash command sequence from flash to RAM
			flashSSDConfig.CallBack = (PCALLBACK) RelocateFunction(
					(uint32_t) __ram_for_callback, CALLBACK_SIZE,
					(uint32_t) callback);
			pFLASHCOMMANDSEQUENCE g_FlashLaunchCommand =
					(pFLASHCOMMANDSEQUENCE) 0xFFFFFFFF;

			g_FlashLaunchCommand = (pFLASHCOMMANDSEQUENCE) RelocateFunction(
					(uint32_t) ramFunc, LAUNCH_CMD_SIZE,
					(uint32_t) FlashCommandSequence);

			// take image
			camera_init();
			capture();
			image_length = fifo_len();

			// set destination to part way into the PFlash memory
			destination = flashSSDConfig.PFlashBase
					+ (flashSSDConfig.PFlashSize
							- FLASH_BLOCKS * FTFx_PSECTOR_SIZE);
			size = FTFx_PSECTOR_SIZE * FLASH_BLOCKS;

			// flash pointer points to the start of the image
			flash_pointer = (uint8_t *) destination;

			// check if memory is protected
			uint32_t protectStatus;
			ret = PFlashGetProtection(&flashSSDConfig, &protectStatus);
			error_trap(ret);

			// erase sector of PFlash
			INT_SYS_DisableIRQGlobal();
			ret = FlashEraseSector(&flashSSDConfig, destination, size,
					g_FlashLaunchCommand);
			INT_SYS_EnableIRQGlobal();
			error_trap(ret);

			// check how many blocks are needed
			int used_blocks = ceil(image_length / FTFx_PSECTOR_SIZE);

			// for each block that needs to get read in
			for (int j = 0; j < used_blocks; j++) {

				// read in an entire sector worth of the image
				for (uint32_t i = 0; i < FTFx_PSECTOR_SIZE; i++) {
					program_buffer[i] = cam_reg_read(0x3D);
				}

				// set destination to j flash block
				destination = flashSSDConfig.PFlashBase
						+ (flashSSDConfig.PFlashSize
								- (FLASH_BLOCKS - j) * FTFx_PSECTOR_SIZE);

				// size is just a single flash block
				size = FTFx_PSECTOR_SIZE;

				// program memory with program_buffer data
				ret = FlashProgram(&flashSSDConfig, destination, size,
						program_buffer, g_FlashLaunchCommand);
				error_trap(ret);

				// check the data written to the memory
				for (margin_read_level = 1; margin_read_level < 0x2;
						margin_read_level++) {
					ret = FlashProgramCheck(&flashSSDConfig, destination, size,
							program_buffer, &FailAddr, margin_read_level,
							g_FlashLaunchCommand);
					error_trap(ret);
				}
			}

			// change to terminal mode once image is in memory
			mode_select = 10;

			// reset camera fifo
			flush_fifo();
		}

		while (mode_select == 10) {
			uint32_t block_number;
			uint8_t block_arr[3];

			// receive with until there is no timeout
			while (!RFM69_RECEIVE_TIMEOUT(buffer)); // rx packet now in buffer

			// if start command, send image size in bytes
			if ((memcmp(&start_command, buffer, sizeof(uint8_t) * PACKET_SIZE)
					== 0)) {
				block_arr[0] = image_length & 0xFF;
				block_arr[1] = (image_length >> 8) & 0xFF;
				block_arr[2] = (image_length >> 16) & 0xFF;

				// clear the buffer
				memset(buffer, 0, sizeof(uint8_t) * PACKET_SIZE);
				memcpy((uint8_t *) buffer, &block_arr, sizeof(uint8_t) * 3);

				// send the packet
				RFM69_SEND(buffer);
			}

			// if stop command erase the image
			else if ((memcmp(&stop_command, buffer,
					sizeof(uint8_t) * PACKET_SIZE) == 0)) {
				mode_select = 8;
			}

			// if packet request, send the requested packet
			else {
				block_number = (buffer[2] << 16) | (buffer[1] << 8)	| (buffer[0]);
				RFM69_SEND(program_buffer + (block_number * PACKET_SIZE));
			}
		}
	}

	// should never return
	return 0;
}

void callback(void) {
	/* Should not use global variable for functions which need to be
	 * relocated such as callback function. If used, some compiler
	 * such as KEIL will be failed in all flash functions and
	 * the bus error will be triggered.
	 */
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
