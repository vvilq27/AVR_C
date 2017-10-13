/*
 * ADC.c
 *
 *  Created on: Oct 10, 2017
 *      Author: root
 */
#include <avr/io.h>
#include "headers/ADC.h"


void adc_init(void){
	ADMUX |= ( 1<<REFS0 );//Vref - Vcc
	ADCSRA |= ( 1<<ADEN )|( 1<<ADPS1 )|( 1<<ADPS0 ); //ADC on, 64 prescaler
}

uint16_t pomiar(uint8_t channel){
	PIN_PORT(ADC_PORT) &= ~(1<<channel);
	ADMUX = (ADMUX & 0xF8)|channel; // selecting channel (kinda wrong if another channel is called)
	ADCSRA |= (1<<ADSC);// measure start
	while( ADCSRA & (1<<ADSC) ); // wait for meas to complete
	return ADCW; // = ADCH and ADCL as 16 bit data

}
