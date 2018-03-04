/*
 * main.c
 *	This testbench program currently has the goal of having an LED on the K22F dev board toggle based on input over UART
 *  Created on: Jan 30, 2018
 *      Author: quinn
 */

#include "MK22F51212.h"
#include "../GPIO/gpio.h"
#include "../UART/uart.h"
#include "../Camera/camera.h"
#include "../FTM/ftm.h"
#include "../I2C/i2c.h"
#include <stdio.h>

int main(){
	FTM0_init();
	GPIO_init();
	UART1_putty_init();
	init_I2C();
	SPI1_Init(16);
	redLED(0);
	greenLED(0);
	//blueLED(1);
	camera_init();
	enable_fifo();
	//capture();
	while(1){
//		flush_fifo(); // clear fifo flag/flush fifo
//		start_capture();

//		char buf[10];
//		int cap = 0;
//		int len = 0;
//		while(!cap){
//			sprintf(buf, "capture: %d\t fifo: %d\r\n",cap, len);
//			putty_putstr(buf);
//			cap = capture_done();
//			len = fifo_len();
//		}
		capture();


	}
	return 0;
}

