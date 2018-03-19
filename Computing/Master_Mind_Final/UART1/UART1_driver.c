/*
 * uart.c
 *
 *  Created on: Jan 31, 2018
 *      Author: quinn
 */

/*Configure UART1 for use with PuTTY terminal using
 * PTE0 and PTE1.
 */

#include "fsl_device_registers.h"
#define reset 0

void UART1_putty_init() {
	uint16_t temp;
	uint16_t ubd;

	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK; // enable clock to PORTE
	SIM_SCGC4 |= SIM_SCGC4_UART1_MASK; // enable clock to UART1

	PORTE_PCR1 |= PORT_PCR_MUX(3);
	PORTE_PCR0 |= PORT_PCR_MUX(3); // configure bits to be UART1

	UART1_C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK); // disable receive/transfer

	UART1_C1 = 0; // set 8 bits

	// calculate and set the baud rate
	ubd = ((21000 * 1000) / (115200 * 16)); // set baudrate
	temp = UART1_BDH & ~(UART_BDH_SBR(0x1F)); // save existing values in register
	UART1_BDH = temp | ((ubd >> 8) & 0x1F);
	UART1_BDL = ubd & UART_BDL_SBR_MASK;

	UART1_C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK); // enable receive/transfer
}



//Sends a character to PuTTY interface
void putty_putchar(char c) {
	while (!(UART1_S1 & 0x80))
		; // check if the transmit buffer is full, do nothing if so
	UART1_D = c; // if not, write 8 bits to the UART0 data register
	return;
}

//Waits for a character from the PuTTY interface
char putty_getchar(void) {
	while (!(UART1_S1 & 0x20))
		; // check if the receive buffer is empty, do nothing if so
	return UART1_D; // returns the gotten char
}

//Sends a NUL-terminated string to the PuTTY interface
void putty_putstr(char *str) {
	int n = 0;
	while (str[n]) { // is the character a NULL?
		putty_putchar(str[n]); // if not, use putChar to output it
		n++;
	}
	return; // quit when we hit a NULL
}

