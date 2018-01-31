#include "fsl_device_registers.h"
#include "stdio.h"
#include "stdlib.h"

// init the module
void UART1_Init();

// print a single character
void UART1_Putchar(char display_char);

// display string in terminal
void UART1_Putstring(uint8_t num);
