/*
 * i2c.c
 *
 *  Created on: Feb 14, 2018
 *      Author: quinn
 */

#include "fsl_device_registers.h"
#include "../FTM/ftm.h"

void i2c_init(){
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; // enable clock to PORTB
	SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK; // enable clock to I2C0

	PORTB_PCR2 |= PORT_PCR_MUX(2); // mux alt2 to enable scl0
	PORTB_PCR3 |= PORT_PCR_MUX(2); // mux alt2 to enable sda0

	I2C0_C1 = 0; // disable extra features
	I2C0_FLT = 0; // disable a bunch of filtering

	I2C0_F = 0x5F; // scl divider = 240, mul = 2 => baud rate = 104.167 kHz
	I2C0_C1 |= I2C_C1_IICEN_MASK; // enable i2c0
}

void I2C0_IRQHandler(){
	I2C0_FLT |= I2C_FLT_STOPF_MASK | I2C_FLT_STARTF_MASK; // in case the interrupt is from start/stop
	I2C0_S |= I2C_S_IICIF_MASK; // clear interrupt flag
	return;
}

void i2c_write(uint8_t reg, uint8_t dat){
	I2C0_C1 |= I2C_C1_TX_MASK; // set k22 to transmit mode

	while(I2C0_S & I2C_S_BUSY_MASK); // wait until bus isn't busy
	I2C0_C1 |= I2C_C1_MST_MASK; // assert master mode

	//delay_us(5);
	I2C0_D  = 0x60; // camera address write mode

	while(!(I2C0_S & I2C_S_TCF_MASK)); // wait until transfer is complete
	// ~& (I2C0_S & I2C_S_RXAK_MASK)

	I2C0_D = reg; // what register are we writing to
	while(!(I2C0_S & I2C_S_TCF_MASK)); // wait until transfer is complete

	I2C0_D = dat; // send the data
	while(!(I2C0_S & I2C_S_TCF_MASK)); // wait until transfer is complete

	I2C0_FLT &= !I2C_FLT_STARTF_MASK;
	I2C0_C1 &= !I2C_C1_MST_MASK; // reset the k22 as a slave, send stop signal
	I2C0_C1 &= !I2C_C1_TX_MASK;
}

uint8_t i2c_read(){
	return 0;
}


