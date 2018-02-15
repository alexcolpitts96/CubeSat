/*
 * SPI0.h
 *
 *  Created on: Jan 31, 2018
 *      Author: quinn
 */

#ifndef SPI_SD_SPI0_H_
#define SPI_SD_SPI0_H_

void SPI0_Init(int frame_size);

void SPI0_Prep();

uint16_t SPI0_RX();

void SPI0_TX(uint16_t tx_data);


#endif /* SPI_SD_SPI0_H_ */
