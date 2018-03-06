//#include "fsl_device_registers.h"
#include "MK22F51212.h"
#include <stdio.h>
#include <stdlib.h>

#include "i2c.h"
#include "uart.h"

int main(){
	uint16_t out_x_MSB=0;
	uint16_t out_x_LSB=0;

	uint16_t out_y_MSB=0;
	uint16_t out_y_LSB=0;

	uint16_t out_z_MSB=0;
	uint16_t out_z_LSB=0;

	uint16_t x=0, y=0, z=0;

	UART1_putty_init();
	init_I2C();


	while(1){
		out_x_MSB=I2CReadRegister(0x01);
		out_x_LSB=I2CReadRegister(0x02);

		out_y_MSB=I2CReadRegister(0x03);
		out_y_LSB=I2CReadRegister(0x04);

		out_z_MSB=I2CReadRegister(0x05);
		out_z_LSB=I2CReadRegister(0x06);

		x=(out_x_MSB << 8) | out_x_LSB;
		y=(out_y_MSB << 8) | out_y_LSB;
		z=(out_z_MSB << 8) | out_z_LSB;

		char data[60];

		sprintf(data," x direction: %d\n\r y direction: %d\n\r z direction: %d\n\r",x,y,z);
		putty_putstr(data);

	}

	return 0;
}
