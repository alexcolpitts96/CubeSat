/*
 * ftm.c
 *
 *  Created on: Feb 26, 2018
 *      Author: quinn
 */

#include "ftm.h"
#include "fsl_device_registers.h"

// information on page 906 of sub-family reference manual
void FTM0_init() {
	// turn on system clock
	SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;

	// no overflow interrupt by default
	// upcounting by default
	FTM0_SC = 0;

	// select system clock (0b01), prescaler of 1 (000)
	FTM0_SC |= FTM_SC_CLKS(0b01) | FTM_SC_PS(0b000);

	// count to 50 ( 1us )
	FTM0_MOD = FTM_MOD_MOD(50);

	// start counter at 0
	FTM0_CNT = 0;

	return;
}

// reset the counter to zero
void FTM0_CNT_RESET() {
	FTM0_SC &= ~FTM_SC_TOF_MASK; // write a 0 to reset TOF flag
	FTM0_CNT = 0;

	return;
}

void delay_us(int time) {
	int overflow_count = 0;
	while(overflow_count<time){
		while (FTM0_SC & FTM_SC_TOF_MASK); // flag will be set to 1 if overflowed
		FTM0_CNT_RESET();
		overflow_count++;
	}
	return;
}
