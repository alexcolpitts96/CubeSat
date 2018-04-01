/*
 * wheel.c
 *
 *  Created on: Mar 22, 2018
 *      Author: quinn
 */

#include "fsl_device_registers.h"
#include "wheel.h"
#include <math.h>

// from kinetis K64 sample
void FTM3_IRQHandler(){
	unsigned long ChannelValue = FTM3_C4V; /*Take the value of the Channel to
	compare it*/
	(void)FTM3_SC;
	FTM3_SC |= 0x0080;
	if(ChannelValue < 32000){
		FTM3_C4V += 1;
	}
	else{
		FTM3_C4V = 0;
	}
}

// set up pwm pins for drv8833
void motor_init(){
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	SIM_SCGC6 |= SIM_SCGC6_FTM3_MASK;

	PORTC_PCR8 |= PORT_PCR_MUX(3); // ftm
	PORTC_PCR9 |= PORT_PCR_MUX(1); // gpio

	GPIOC_PDDR |= 1<<9;

	FTM3_SC |= 0x004C;
	FTM3_MOD = 32;
	FTM3_C4SC |= 0x0028;
	FTM3_C4V = 1;
}

// switch motor direction
void motor_setdir(int dir){
	switch(dir){
	case MOTOR_CCW:
		GPIOC_PCOR |= 1<<9;
		break;
	case MOTOR_CW:
		GPIOC_PSOR |= 1<<9;
		break;
	}
	return;
}

// change motor duty cycle
void motor_setspeed(float dcycle){
	if(dcycle>1){
		return;
	}else{
		FTM3_C4V = (int)(floor(dcycle*32.0f));
	}
	return;
}

// stop the motor properly
void motor_stop(){
	FTM3_C4V = 0;
	return;
}

// rotate deg degrees
// probably won't work on earth
void motor_rotate(int deg){

}

// max speed cw 5s, max speed ccw 5s
// used for demo
void motor_testcase(){

}
