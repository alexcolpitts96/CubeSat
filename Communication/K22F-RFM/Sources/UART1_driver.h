#include "fsl_device_registers.h"
#include "stdio.h"
#include "stdlib.h"

void UART1_Init(){
	// initialize UART for PC display
	// no external connections required

	// clock enables for GPIO and UART
	SIM_SCGC4 |= SIM_SCGC4_UART1_MASK; // enable UART 1
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; // enable GPIO C

	// mux pins for GPIO and UART
	PORTC_PCR3 |= PORT_PCR_MUX(3); // RX, C3
	PORTC_PCR4 |= PORT_PCR_MUX(3); // TX, C4

	// calculate baud rate register value using ((21000*1000)/(baud_rate * 16))
	UART1_BDL |= 0x89;		// setting baud rate for UART0 to 9600

	// control registers for UART
	UART1_C1 |= 0; // no parity
	UART1_C2 |= UART_C2_RE_MASK | UART_C2_TE_MASK; // enable transmit and receive
	UART1_C3 |= UART_C3_PEIE_MASK | UART_C3_FEIE_MASK;
}

// print a single character
void UART1_Putchar(char display_char){

	while(!(UART1_S1 & UART_S1_TC_MASK)){} // wait until the tx is ready for next char

	UART1_D = display_char; // when ready send next char
}

// display string in terminal
void UART1_Putstring(uint8_t num){
	int i = 0;
	char welcome[80] = "Test  \n\0";
	welcome[5] = num;

	while(welcome[i]){
		UART1_Putchar(welcome[i]);
		i++;
	}
}
