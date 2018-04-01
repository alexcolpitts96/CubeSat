/*
 * accel.c
 *
 *  Created on: Mar 7, 2018
 *      Author: quinn
 */

#include "../I2C/i2c.h"
#include "accel.h"
#include <math.h>


void accel_init(){
	I2CWriteRegister(ACCEL_SLAVE_ADDRESS,ACCEL_CTRL_REG1,0x00); // write 0x00 to set standby, thus enabling changes
	I2CWriteRegister(ACCEL_SLAVE_ADDRESS,ACCEL_M_CTRL_REG1,0x1F); // enable magnetometer & noise reduction
	I2CWriteRegister(ACCEL_SLAVE_ADDRESS,ACCEL_M_CTRL_REG2,0x20);  // configure magnetometer
	I2CWriteRegister(ACCEL_SLAVE_ADDRESS,ACCEL_XYZ_DATA_CFG,0x01); // configure accelerometer
	I2CWriteRegister(ACCEL_SLAVE_ADDRESS,ACCEL_CTRL_REG1,0x0D); // enable module
	return;
}

void sensor_read(struct sensor_dat *data){
	uint8_t buf[13];

	(I2CReadMultiRegisters(ACCEL_SLAVE_ADDRESS,ACCEL_STATUS,buf,13));

	(data->dat_X) = (int16_t) ((int16_t)(buf[1]<<8)|(int16_t)buf[2])>>2;
	(data->dat_Y) = (int16_t) ((int16_t)(buf[3]<<8)|(int16_t)buf[4])>>2;
	(data->dat_Z) = (int16_t) ((buf[5]<<8)|buf[6])>>2;

}

// uses factory calibration settings. may not be 100% accurate yet
void MPU_init(){
	I2CWriteRegister(MPU_SLAVE_ADDRESS,MPU_PWR_MGMT1,0x01); // enable module, clock select
	I2CWriteRegister(MPU_SLAVE_ADDRESS,MPU_PWR_MGMT2,0x30); // disable accel, enable gyro
	I2CWriteRegister(MPU_SLAVE_ADDRESS,MPU_CFG,0x03); // sample rate
	I2CWriteRegister(MPU_SLAVE_ADDRESS,MPU_SMPLRT,0x04); // 200 Hz div select
	I2CWriteRegister(MPU_SLAVE_ADDRESS,MPU_INT_ENABLE,0x00); // disable interrupt
	I2CWriteRegister(MPU_SLAVE_ADDRESS,MPU_GYRO_CFG,0x00); // enable gyro
	return;
}

void MPU_gyro_read(struct sensor_dat *data){
	for(int i=0;i<500;i++) Pause();
	uint8_t buf[6];

	(I2CReadMultiRegisters(MPU_SLAVE_ADDRESS,MPU_OUT_X_MSB,buf,6));

	(data->dat_X) = (int16_t) ((buf[0]<<8)|buf[1]);
	(data->dat_Y) = (int16_t) ((buf[2]<<8)|buf[3]);
	(data->dat_Z) = (int16_t) ((buf[4]<<8)|buf[5]);
}

int check_tumble(){
	struct sensor_dat data;
	(data.dat_X) = 0;
	(data.dat_Y) = 0;
	(data.dat_Z) = 0;

	int long sum = 0;
	MPU_gyro_read(&data);
	sum = sqrt((pow(data.dat_X,2)) + (pow(data.dat_Y,2)) + (pow(data.dat_Z,2)));

	if(sum>TUMBLE_THRESH){
		return 1;
	}else{
		return 0;
	}
}
