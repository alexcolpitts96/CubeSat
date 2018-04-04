/*
 * accel.h
 *
 *  Created on: Mar 7, 2018
 *      Author: quinn
 */

#ifndef ACCEL_ACCEL_H_
#define ACCEL_ACCEL_H_

//#define TUMBLE_THRESH 10000
#define TUMBLE_THRESH 1000

// FXOS8700CQ I2C address
#define ACCEL_SLAVE_ADDRESS 0x38 // 0x1C<<1
#define MPU_SLAVE_ADDRESS 0x68<<1

// FX0S8700CQ register definitions
#define ACCEL_STATUS 0x00
#define ACCEL_WHOAMI 0x0D
#define ACCEL_XYZ_DATA_CFG 0x0E
#define ACCEL_CTRL_REG1 0x2A
#define ACCEL_M_CTRL_REG1 0x5B
#define ACCEL_M_CTRL_REG2 0x5C
#define ACCEL_WHOAMI_VAL 0xC7
#define ACCEL_OUT_X_MSB 0x01
#define ACCEL_OUT_X_LSB 0x02
#define ACCEL_OUT_Y_MSB 0x03
#define ACCEL_OUT_Y_LSB 0x04
#define ACCEL_OUT_Z_MSB 0x05
#define ACCEL_OUT_Z_LSB 0x06

// MPU register definitions
#define MPU_OUT_X_MSB 0x43
#define MPU_OUT_X_LSB 0x44
#define MPU_OUT_Y_MSB 0x45
#define MPU_OUT_Y_LSB 0x46
#define MPU_OUT_Z_MSB 0x47
#define MPU_OUT_Z_LSB 0x48
#define MPU_PWR_MGMT2 0x6C
#define MPU_PWR_MGMT1 0x6B
#define MPU_CFG 0x1A
#define MPU_SMPLRT 0x19
#define MPU_GYRO_CFG 0x1B
#define MPU_INT_ENABLE 0x38


typedef struct sensor_dat{
	int16_t dat_X;
	int16_t dat_Y;
	int16_t dat_Z;
}sense;

void accel_init();

void sensor_read(struct sensor_dat *data);

void MPU_init();

void MPU_gyro_read();

int check_tumble();

#endif /* ACCEL_ACCEL_H_ */
