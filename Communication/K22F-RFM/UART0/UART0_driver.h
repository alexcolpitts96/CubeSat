#include "fsl_device_registers.h"
#include "stdio.h"
#include "stdlib.h"

// init the module
void UART0_Init();

// print a single character
void UART0_Putchar(char display_char);

// display string in terminal
void UART0_Putstring();
