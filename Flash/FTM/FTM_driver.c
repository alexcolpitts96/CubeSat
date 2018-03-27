#include "FTM_driver.h"
#include "fsl_device_registers.h"
#include "../UART1/UART1_driver.h"

// information on page 906 of sub-family reference manual
void FTM0_init() {
	// turn on system clock
	SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;

	// no overflow interrupt by default
	// upcounting by default

	// select system clock (0b01), prescaler of 64 (110)
	//FTM0_SC |= FTM_SC_CLKS(0b01) | FTM_SC_PS(0b101);
	FTM0_SC |= FTM_SC_CLKS(0b10) | FTM_SC_PS(0b001);

	// time to transmit and receive from the satellite is roughly 2.6 ms to 9.3 ms, give longer interval of 50 ms
	//FTM0_MOD = FTM_MOD_MOD(0xF424);
	FTM0_MOD = FTM_MOD_MOD(0x5FFF);

	// start counter at 0
	FTM0_CNT = 0;

	return;
}

void FTM1_init() {
	// turn on system clock
	SIM_SCGC6 |= SIM_SCGC6_FTM1_MASK;

	// no overflow interrupt by default
	// upcounting by default

	// select system clock (0b01), prescaler of 1 (000)
	FTM1_SC |= FTM_SC_CLKS(0b01) | FTM_SC_PS(0b000);

	FTM1_MOD = FTM_MOD_MOD(24); // 1 us before it overflows

	// start counter at 0
	FTM1_CNT = 0;

	return;
}

// reset the counter to zero
void FTM0_CNT_RESET() {
	FTM0_SC &= ~FTM_SC_TOF_MASK; // write a 0 to reset TOF flag
	FTM0_CNT = 0;

	return;
}

// reset the counter to zero
void FTM1_CNT_RESET() {
	FTM1_SC &= ~FTM_SC_TOF_MASK; // write a 0 to reset TOF flag
	FTM1_CNT = 0;

	return;
}

uint8_t FTM0_WAIT() {
// if timer has overflown return 1
	if (FTM0_SC & FTM_SC_TOF_MASK) { // flag will be set to 1 if overflown
		return 1;
	}

// else return 1
	else {
		return 0;
	}
}

uint8_t FTM1_WAIT() {
// if timer has overflown return 1
	if (FTM1_SC & FTM_SC_TOF_MASK) { // flag will be set to 1 if overflown
		return 1;
	}

// else return 1
	else {
		return 0;
	}
}

// delay for integer a known number of microseconds (us)
void FTM1_delay(int n) {
	int i;

	// wait as many 1 us intervals as needed
	for(i = 0; i < n; i++){
		FTM1_CNT_RESET(); // reset the counter
		while(!FTM1_WAIT()); // wait for 1 us
	}

	return;
}
