/*
 * FTM Driver
 *
 * Author: Alex Colpitts
 *
 * Date: February 16, 2018
 */

#ifndef SOURCES_FTM_FTM_H
#define SOURCES_FTM_FTM_H

#include "fsl_device_registers.h"

// information on page 906 of sub-family reference manual
void FTM0_init();

void FTM1_init();

// reset the counter to zero
void FTM0_CNT_RESET();

// reset the counter to zero
void FTM1_CNT_RESET();

uint8_t FTM0_WAIT();

uint8_t FTM1_WAIT();

// delay for integer a known number of microseconds (us)
void FTM1_delay(int n);

#endif // SOURCES_FTM_FTM_H
