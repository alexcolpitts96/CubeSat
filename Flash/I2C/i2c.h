/*
 * All software, source code, included documentation, and any implied know-how
 * are property of Freescale Semiconductor and therefore considered
 * CONFIDENTIAL INFORMATION.
 * This confidential information is disclosed FOR DEMONSTRATION PURPOSES ONLY.
 *
 * All Confidential Information remains the property of Freescale Semiconductor
 * and will not be copied or reproduced without the express written permission
 * of the Discloser, except for copies that are absolutely necessary in order
 * to fulfill the Purpose.
 *
 * Services performed by FREESCALE in this matter are performed AS IS and
 * without any warranty.
 * CUSTOMER retains the final decision relative to the total design and
 * functionality of the end product.
 * FREESCALE neither guarantees nor will be held liable by CUSTOMER for the
 * success of this project.
 *
 * FREESCALE disclaims all warranties, express, implied or statutory
 * including, but not limited to, implied warranty of merchantability or
 * fitness for a particular purpose on any hardware, software ore advise
 * supplied to the project by FREESCALE, and or any product resulting from
 * FREESCALE services.
 * In no event shall FREESCALE be liable for incidental or consequential
 * damages arising out of this agreement. CUSTOMER agrees to hold FREESCALE
 * harmless against any and all claims demands or actions by anyone on account
 * of any damage,or injury, whether commercial, contractual, or tortuous,
 * rising directly or indirectly as a result of the advise or assistance
 * supplied CUSTOMER in connectionwith product, services or goods supplied
 * under this Agreement.
 *
 * Modified by Quinn Selby based on Freescale i2c libraries for k64f.
 */

#ifndef __I2C_H
#define __I2C_H 1

#include "fsl_device_registers.h"

#define i2c_DisableAck()		I2C0_C1 |= I2C_C1_TXAK_MASK

#define i2c_RepeatedStart()		I2C0_C1 |= I2C_C1_RSTA_MASK;

#define i2c_Start()				I2C0_C1 |= I2C_C1_TX_MASK;\
								I2C0_C1 |= I2C_C1_MST_MASK

#define i2c_Stop()				I2C0_C1 &= ~I2C_C1_MST_MASK;\
								I2C0_C1 &= ~I2C_C1_TX_MASK

#define i2c_EnterRxMode()		I2C0_C1 &= ~I2C_C1_TX_MASK;\
								I2C0_C1 &= ~I2C_C1_TXAK_MASK

#define i2c_Wait()				while((I2C0_S & I2C_S_IICIF_MASK)==0) {} \
								I2C0_S |= I2C_S_IICIF_MASK;

#define i2c_write_byte(data)	I2C0_D = data

#define MWSR					0x00	/* Master write  */
#define MRSW					0x01	/* Master read */

void init_I2C(void);
void IIC_StartTransmission(unsigned char SlaveID, unsigned char Mode);

void Pause(void);

void I2CWriteRegister(unsigned char SlaveID, unsigned char u8RegisterAddress,
		unsigned char u8Data);
unsigned char I2CReadRegister(unsigned char SlaveID,
		unsigned char u8RegisterAddress);
void I2CReadMultiRegisters(unsigned char SlaveID,
		unsigned char u8RegisterAddress, uint8_t* buf, unsigned char bytes);
void I2C0_IRQHandler();

#endif		/* __I2C_H */
