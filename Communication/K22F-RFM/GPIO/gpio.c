/*
 * gpio.c
 *
 *  Created on: Jan 30, 2018
 *      Author: quinn
 */

#include "fsl_device_registers.h"

void GPIO_init(){
	//SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK; // enable clock to port A
	//SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; // enable clock to port B
	// SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; // enable clock to port C
	//SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK; // enable clock to port D
	// SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK; // enable clock to port E

	//PORTD_PCR5 |= PORT_PCR_MUX(1);
	//PORTA_PCR1 |= PORT_PCR_MUX(1);
	//PORTB_PCR17 |= PORT_PCR_MUX(1);

	//GPIOA_PDDR = 0 << 5// no need as set to input by default
	//GPIOD_PDDR = 1<<5;
	//GPIOB_PDDR &= 0<<17;

	//GPIOA_PCOR = -1;

	return;
}

void greenLED(int state){
	if(state){
		GPIOA_PCOR = 1<<1;
	}else{
		GPIOA_PSOR = 1<<1;
	}
}

void redLED(int state){
	if(state){
		GPIOA_PCOR = 1<<2;
	}else{
		GPIOA_PSOR = 1<<2;
	}
}

void blueLED(int state){
	if(state){
		GPIOD_PCOR = 1<<5;
	}else{
		GPIOD_PSOR = 1<<5;
	}
}

int checkSW3(){
	if(GPIOB_PDIR & 1<<17) return 0;
	return 1;
}

void RFM69_DIO0_Init(){
	// pick system clock module as needed
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK; // enable clock to port A

	// mux pins to be external
	PORTA_PCR12 |= PORT_PCR_MUX(1); // set as GPIO, A5

	// set PDDR to receive (input is zero by default)
}

int RFM69_DIO0_Read(){
	int read;

	// check receive register (corresponding GPIO pin)
	read = GPIOA_PDIR & (1 << 12); // mask all but the A5 pin

	return read;
}
