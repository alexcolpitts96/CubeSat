#include "fsl_device_registers.h"
#include "stdio.h"
#include "stdlib.h"

#define reset 0

void UART0_Init(){
	// initialize UART for PC display
	// no external connections required

	// clock enables for GPIO and UART
	SIM_SCGC4 |= SIM_SCGC4_UART0_MASK; // enable UART
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; // enable GPIO B

	// mux pins for GPIO and UART
	PORTB_PCR16 |= PORT_PCR_MUX(3); // RX, B16
	PORTB_PCR17 |= PORT_PCR_MUX(3); // TX, B17

	// calculate baud rate register value using ((21000*1000)/(baud_rate * 16))
	UART0_BDL |= 0x89;		// setting baud rate for UART0 to 9600

	// control registers for UART
	UART0_C1 |= 0; // no parity
	UART0_C2 |= UART_C2_RE_MASK | UART_C2_TE_MASK; // enable transmit and receive
	UART0_C3 |= UART_C3_PEIE_MASK | UART_C3_FEIE_MASK;
}


// print a single character
void UART0_Putchar(char display_char){

	while(!(UART0_S1 & UART_S1_TC_MASK)){} // wait until the tx is ready for next char

	UART0_D = display_char; // when ready send next char
}

// display string in terminal
void UART0_Putstring() {
	int i = 0;
	char welcome[80] = "Welcome to our program\0";

	while(welcome[i]){
		UART0_Putchar(welcome[i]);
		i++;
	}
}
