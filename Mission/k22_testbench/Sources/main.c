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
#include "../ACCEL/accel.h"
#include "../SPI/spi.h"
#include "../WHEEL/wheel.h"
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

	motor_init();

	MPU_init();

	struct sensor_dat mydat;
	mydat.dat_X = 0;
	mydat.dat_Y = 0;
	mydat.dat_Z = 0;
	int tmbl = 0;

	while(1){
		MPU_gyro_read(&mydat);
		tmbl = check_tumble();
		char buf[80];
		sprintf(buf, "\033c x: %d \n\r y: %d \n\r z: %d \n\r tumbling: %d \n\r",mydat.dat_X,mydat.dat_Y,mydat.dat_Z,tmbl);
		putty_putstr(buf);
		//for(int i=0;i<1000;i++){
		//	Pause();
		//}
	}


	return 0;
}

