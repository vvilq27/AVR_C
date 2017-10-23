/*
 * SPI.h
 *
 *  Created on: Oct 19, 2017
 *      Author: root
 */
#include "common.h"

#ifndef HEADERS_SPI_H_
#define HEADERS_SPI_H_

#define MOSI PB3
#define MISO PB4
#define	SCK PB5
#define CS PB2


void SPI_init(void);
void SPI_send(uint8 );
uint8 SPI_receive(void);

#endif /* HEADERS_SPI_H_ */
