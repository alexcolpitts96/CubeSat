/*
 * camera.c
 *
 *  Created on: Feb 2, 2018
 *      Author: quinn
 *
 *      SPI drivers adapted from Alex Colpitts' SPI0 drivers.
 *
 *      All other functions are specific to handling the camera module.
 */

#include "fsl_device_registers.h"
#include "camera.h"
#include "../I2C/i2c.h"
#include "../Comms/Comms.h"
#include "math.h"
#include "../SPI/spi.h"

void cam_cfg(struct ov2640_reg_cfg *vals) {
	while (((vals->reg) != 0xff) || ((vals->val) != 0xff)) { // both values aren't 0xff
		I2CWriteRegister(vals->reg, vals->val);
		vals++;
	}
	return;
}

// currently: JPEG format, 1600x1200
void camera_init() {
	I2CWriteRegister(0xff, 0x01);
	I2CWriteRegister(0x12, 0x80);
	cam_cfg(JPEG_INIT); // jpeg init
	cam_cfg(YUV_422); // yuv color select
	cam_cfg(JPEG); // jpeg select
	I2CWriteRegister(0xff, 0x01);
	I2CWriteRegister(0x15, 0x00);
	//cam_cfg(JPEG_SMALL); // 320x240 init
	cam_cfg(JPEG_LARGE); // 1600x1200 select
	return;
}

uint8_t cam_reg_read(uint8_t regaddr) {
	uint8_t tmp;
	SPI1_TX(regaddr << 8,0);
	tmp = SPI1_RX();
	Pause();

	return tmp;
}

uint32_t fifo_len() {
	uint32_t len1 = cam_reg_read(0x42); // bits 7-0
	uint32_t len2 = cam_reg_read(0x43); // bits 15-8
	uint32_t len3 = cam_reg_read(0x44); // bits 18-16
	return ((len3 << 16) | (len2 << 8) | len1) & 0x07fffff; // combine them, return length of fifo in
}

int capture_done() {
	return (cam_reg_read(0x41) & 0x08) >> 3; // return 1 for capture done, 0 otherwise
}

// read in the entire fifo and return a pointer to it in memory
void fifo_read() {
	// check fifo length for errors
	uint32_t len = fifo_len();
	if ((len == 0) || (len >= MAX_FIFO_LENGTH)) {
		return; // ERROR
	}

	//int read_count = 0;

	//int packet_number = (int) ceil((float) len / (float) PACKET_SIZE);

	/*
	 for (int i = 0; i < packet_number; i++) {
	 for (int j = 0; j < PACKET_SIZE; j++) {

	 // if all bytes have been read return
	 if (read_count >= len) {
	 return;
	 }

	 // read into memory
	 image[i][j] = cam_reg_read(0x3D);
	 read_count++;
	 }
	 }
	 //*/
	return;
}

// sends capture command and returns pointer to start of image byte array (quite large)
void capture() {

	enable_fifo();
	flush_fifo(); // clear fifo flag/flush fifo
	start_capture()
	;
	while (!(capture_done()))
		; // check flag for capture complete
	//fifo_read(image); // read entire fifo in

	//flush_fifo(); // finished reading, so empty fifo
	return;
}
