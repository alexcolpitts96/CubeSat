#include "FTM_driver.h"
#include "fsl_device_registers.h"
#include "../UART1/UART1_driver.h"

// information on page 906 of sub-family reference manual
void FTM0_init(){
	// turn on system clock
	SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;

	// no overflow interrupt by default
	// upcounting by default

	// select system clock (0b01), prescaler of 64 (110)
	FTM0_SC |= FTM_SC_CLKS(0b01) | FTM_SC_PS(0b101);

	// time to transmit and receive from the satellite is roughly 2.6 ms to 9.3 ms, give longer interval of 50 ms
	FTM0_MOD = FTM_MOD_MOD(0xF424);

	// start counter at 0
	FTM0_CNT = 0;

	return;
}

// reset the counter to zero
void FTM0_CNT_RESET(){
	FTM0_SC &= ~FTM_SC_TOF_MASK; // write a 0 to reset TOF flag
	FTM0_CNT = 0;

	return;
}

uint8_t FTM0_WAIT(){
	// if timer has overflown return 1
	if(FTM0_SC & FTM_SC_TOF_MASK){ // flag will be set to 1 if overflown
		return 1;
	}

	// else return 1
	else{
		return 0;
	}
}
