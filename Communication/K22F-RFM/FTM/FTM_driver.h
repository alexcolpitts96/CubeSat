#include "fsl_device_registers.h"

// information on page 906 of sub-family reference manual
void FTM0_init();

// reset the counter to zero
void FTM0_CNT_RESET();

// return 1 while not overflown, return 0 when overflow occurs
uint8_t FTM0_WAIT();
