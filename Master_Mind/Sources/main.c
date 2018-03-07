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

#define MAX_IMAGE_SIZE 0x5FFFF

void master_init() {
	UART0_Init();
	UART1_putty_init();
	SPI0_Init(16);
	RFM69_DIO0_Init();
	RFM69_Init(); // must always be after the SPI interface has been enabled
	FTM0_init();
	//FTM1_init();
	init_I2C();
	SPI1_Init(16);
	camera_init();
}

int main() {
	uint8_t *buffer = (uint8_t *) calloc(PACKET_SIZE, sizeof(uint8_t));
	uint8_t *camera = (uint8_t *) calloc(PACKET_SIZE, sizeof(uint8_t));

	uint8_t **storage = (uint8_t **) calloc(10000, sizeof(uint8_t *));
	for(int i = 0; i < 10000; i++){
		storage[i] = (uint8_t *) calloc(PACKET_SIZE, sizeof(uint8_t));
	}

	int image_length;
	uint32_t last_packet;

	master_init();

	// take image
	capture();
	//image_length = (int) ceil((float) fifo_len() / (float) PACKET_SIZE);
	image_length = fifo_len();

	// transmit size of the image and wait for the start command
	imageSize(buffer, image_length);

	///*
	last_packet = 0;
	while ((strcmp((char *) &stop_command, (char *) buffer) != 0)) {
		last_packet = transmitPacket(buffer, camera, last_packet);
	}
	//*/

	/*// read in next packet from camera
	 for (int i = 0; i < fifo_len(); i++) {
	 putty_putchar(cam_reg_read(0x3D));
	 }
	 //*/

	free(camera);
	free(buffer);

	return 0;
}
