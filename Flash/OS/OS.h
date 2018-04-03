/*
 * OS_Function.h
 *
 *  Created on: Mar 21, 2018
 *      Author: Hsuan-Wei Lo
 */

#ifndef SOURCES_OS_FUNCTION_H_
#define SOURCES_OS_FUNCTION_H_

void init_sys_module();
void disable_modules();
void sleep_now();
uint8_t check_bat();
uint8_t check_solar();
void deinit_sys_module();
void LED();
void LED1();
void ADC0_Init();
float ADC0_Convert();
void ADC1_Init();
float ADC1_Convert();
void LPTMR0_enable(int miliseconds);
void WDT_int();
void Refresh_Dog();
void Unleash_Dog();
void MOSFET_ON_Cam();
void MOSFET_OFF_Cam();
void MOSFET_ON_RF();
void MOSFET_OFF_RF();

#endif /* SOURCES_OS_FUNCTION_H_ */
