/*
 * main.c
 *
 *  Created on: Oct 8, 2017
 *      Author: root
 */
//#include <avr/pgmspace.h>
//#include <avr/eeprom.h>
#include "headers/LCD.h"

int main(void){
	DDRD |= _BV(7);
	adc_init();
	lcd_init();
	lcd_cls();

	char tab[] = "yoyo w? ";
	lcd_str(tab);
	lcd_write_data(98);
//	lcd_sendHalf(99); // cant use this here since static method

	while(1){
		lcd_cls();
		lcd_int(pomiar(0));
		PORTD ^= (1<<PD7);
		_delay_ms(1000);

	}
}
