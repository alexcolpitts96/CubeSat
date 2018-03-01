/*
 * i2c.h
 *
 *  Created on: Feb 14, 2018
 *      Author: quinn
 */

#ifndef I2C_I2C_H_
#define I2C_I2C_H_

void i2c_init();

void I2C0_IRQHandler();

void i2c_write(uint8_t addr, uint8_t dat);

uint8_t i2c_read();

#endif /* I2C_I2C_H_ */
