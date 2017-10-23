/*
 * SPI.c
 *
 *  Created on: Oct 19, 2017
 *      Author: root
 */
#include <avr/io.h>
#include "headers/SPI.h"
#include "headers/common.h"

void SPI_init(){
	DDRB |= (1<<MOSI)|(1<<SCK)|(1<<CS);
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR1);		//enable SPI, set as master,
	PORTB |= (1 << CS) | (1<<MISO);
}

void SPI_send(uint8 data){
	SPDR = data;
	while( !( SPSR & (1<<SPIF) ) );				//wait until interrupt flag *(set after previous data transfer complete) is removed
}

uint8 SPI_receive(){
	while( !(SPSR & (1<<SPIF) ) );
	return SPDR;
}

uint8 SPI_transfer_byte(uint8 data){
	SPDR = data;
	while( !(SPSR & (1<<SPIF) ) );
	// replace  2 upper lines with SPI_send if works the same
	return SPDR;
}

void SPI_init_slave(){
	DDRB |= (1<<MISO);							//miso as output
	SPCR |= (1<<SPE);							//spi enable
}
