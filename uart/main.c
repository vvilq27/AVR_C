/*
 * main.c
 *
 *  Created on: Oct 8, 2017
 *      Author: root
 */
//#include <avr/pgmspace.h>
//#include <avr/eeprom.h>

// PROBLEM: nie widzi __UBRR z usart.h
//#define F_CPU 8000000UL

#include <avr/interrupt.h>
#include "headers/LCD.h"
#include "headers/USART.h"

int main(void){
	DDRD |= _BV(7);
	sei();
//	adc_init();
	lcd_init();
//	USART_Init(103);
	lcd_cls();

//	UBRR0H = (uint8_t) (baud>>8);

	char tab = "";
//	lcd_write_data(98);
//	lcd_sendHalf(99); // cant use this here since static method

	while(1){
		/*tab = uart_get_char();
		lcd_cls();
		lcd_str("dane:");
		lcd_str(tab);
		lcd_str(" ");
		lcd_int(UART_RxTail);
		lcd_str(" ");
		lcd_int(UART_RxHead);*/
//		uart_put_char("P");
//		uart_put_char("P");
//		uart_put_char("P");
//		lcd_cls();
//		lcd_hex(UDR0);
//		lcd_int(__UBRR);
		PORTD ^= (1<<PD7);
		_delay_ms(1000);

	}
}
