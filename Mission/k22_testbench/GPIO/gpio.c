/*
 * gpio.c
 *
 *  Created on: Jan 30, 2018
 *      Author: quinn
 */

#include "fsl_device_registers.h"

void GPIO_init(){
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK; // enable clock to port A
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; // enable clock to port B
	// SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; // enable clock to port C
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK; // enable clock to port D
	// SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK; // enable clock to port E

	PORTD_PCR5 |= PORT_PCR_MUX(1);
	PORTA_PCR1 |= PORT_PCR_MUX(1);
	PORTA_PCR2 |= PORT_PCR_MUX(1);
	PORTB_PCR17 |= PORT_PCR_MUX(1);
	PORTB_PCR0 |= PORT_PCR_MUX(1);

	GPIOD_PDDR = 1<<5;
	GPIOA_PDDR = 0b11;
	GPIOB_PDDR &= 0<<17;
	GPIOB_PDDR |= 1;

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
