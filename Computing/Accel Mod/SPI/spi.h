/*
 * spi.h
 *
 *  Created on: Mar 9, 2018
 *      Author: quinn
 */

#ifndef SPI_SPI_H_
#define SPI_SPI_H_

void SPI1_Init(int frame_size);
void SPI1_txPrep();
uint16_t SPI1_RX();
void SPI1_TX(uint16_t tx_data, int cs);
uint8_t SPI1_read(uint8_t regaddr);

#endif /* SPI_SPI_H_ */
