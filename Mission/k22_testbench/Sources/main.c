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
	//i2c_init();
	GPIO_init();
	UART1_putty_init();
	init_I2C();
	//SPI1_Init(16);
	redLED(0);
	greenLED(0);
	//blueLED(1);
	while(1){

		while(1){
			I2CWriteRegister(0xff,0x00);

		}
		putty_putstr("a");
		//i2c_write(0xFF, 0x01); // write 0x01 to register 0xFF
		//putty_putchar('a');
		//SPI1_TX(0x80FA);
		//rec = SPI1_read(0x00);
		//char buf[8];
		//sprintf(buf,"0x%X\n\r",rec);
		//putty_putstr(buf);
	}
	return 0;
}

