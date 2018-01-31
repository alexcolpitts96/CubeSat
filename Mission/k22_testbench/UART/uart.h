/*
 * uart.h
 *
 *  Created on: Jan 31, 2018
 *      Author: quinn
 */

#ifndef UART_UART_H_
#define UART_UART_H_

void UART1_putty_init();

void putty_putchar(char c);

char putty_getchar(void);

void putty_putstr(char *str);

#endif /* UART_UART_H_ */
