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
#include <stdio.h>

int main(){
	GPIO_init();
	UART1_putty_init();
	SPI1_Init(16);
	redLED(0);
	greenLED(0);
	blueLED(0);
	char mychr;
	// uint8_t rec;
	while(1){
		mychr = putty_getchar();
		putty_putchar(mychr);
		if(mychr == 'c'){
			putty_putstr("c\n\r");
			capture();
		}

	}
	return 0;
}

