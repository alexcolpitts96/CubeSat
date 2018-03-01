/*
 * ftm.h
 *
 *  Created on: Feb 26, 2018
 *      Author: quinn
 */

#ifndef SOURCES_FTM_FTM_H
#define SOURCES_FTM_FTM_H

#include "fsl_device_registers.h"

// information on page 906 of sub-family reference manual
void FTM0_init();

// reset the counter to zero
void FTM0_CNT_RESET();

// return 0 while not overflown, return 1 when overflow occurs
void delay_us(int time);

#endif // SOURCES_FTM_FTM_H
