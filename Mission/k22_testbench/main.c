/*
 * main.c
 *	This testbench program currently has the goal of having an LED on the K22F dev board toggle based on input over UART
 *  Created on: Jan 30, 2018
 *      Author: quinn
 */

#include "MK22F51212.h"
#include "GPIO/gpio.h"
#include "UART/uart.h"

int main(){
	GPIO_init();
	UART1_putty_init();
	redLED(0);
	greenLED(0);
	blueLED(0);
	while(1){
		putty_putchar('a');
/*		if(checkSW3()){
			blueLED(1);
		}else{
			blueLED(0);
		}*/
	}


	return 0;
}

