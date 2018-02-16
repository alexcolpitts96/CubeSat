#include "FTM_driver.h"
#include "fsl_device_registers.h"
#include "../UART1/UART1_driver.h"

// information on page 906 of sub-family reference manual
void FTM0_init(){
	// turn on system clock
	SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;

	// no overflow interrupt by default
	// upcounting by default

	// select system clock, prescaler of 128
	FTM0_SC |= FTM_SC_CLKS(0b01) | FTM_SC_PS(0b111);

	// system clock/128 is 666 kHz, 3ms is roughly 2000 cycles (0x7D0)
	// time to transmit and receive from the satellite is roughly 2.6 ms to 9.3 ms
	//FTM0_MOD = FTM_MOD_MOD(0x7D0);
	FTM0_MOD = FTM_MOD_MOD(0xFFFF);

	// start counter at 0
	FTM0_CNT = 0;

	return;
}

// reset the counter to zero
void FTM0_CNT_RESET(){
	FTM0_CNT = 0;

	return;
}

uint8_t FTM0_WAIT(){
	// if time has overflown return 0
	if(FTM0_SC & FTM_SC_TOF_MASK){
		return 0;
	}

	// else return 1
	else{
		return 1;
	}
}
