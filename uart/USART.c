/*
 * USART.c
 *
 *  Created on: Oct 11, 2017
 *      Author: root
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include "headers/USART.h"
#include "headers/LCD.h"

// uses register names declared in iom16.h not iom328p.h like wtf? i've lost 2 days on figuring that out

void USART_Init(unsigned int baud){
	DDRD |= (1<<6);
	//set baud rate
	UBRR0H = (uint8_t) (baud>>8);
	UBRR0L = (uint8_t) (baud);
//	enable receiver and transmitter
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0);
//	set frame format: 8 data bits, 2 stop bits
	UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
//	_SFR_MEM8(0xC2) |= (1<<UCSZ00) | (1<<UCSZ01);


	// display setup parameters
//	lcd_cls();
//	lcd_str("UCSR0A(hex): ");
//	lcd_hex(UCSR0A);
//	_delay_ms(1500);
//	lcd_cls();
//	lcd_str("UCSR0B(hex): ");
//	lcd_hex(UCSR0B);
//	_delay_ms(1500);
//	lcd_cls();
//	lcd_str("UCSR0C(hex): ");
//	lcd_hex(UCSR0C);
//	_delay_ms(1500);
}
//reading received and saved data stored in program buffer
char uart_get_char(void){
	if( UART_RxHead == UART_RxTail ){
//		lcd_str("no char");
//		_delay_ms(1000);
		return 0;// check if indexes are equal => no data in buffer
	}
	else{
		UART_RxTail = (UART_RxTail + 1) & UART_RX_BUFF_MASK;//shifts read register
//		lcd_str("theres char!");
//		_delay_ms(1000);
		return UART_RxBuff[UART_RxTail];//output received data
	}

}
void uart_put_char(char data){
	uint8_t tmp_head;
	tmp_head = (UART_TxHead + 1) & UART_TX_BUFF_MASK;
	while( tmp_head == UART_TxTail ){
		//TODO:
	}
	UART_TxBuff[tmp_head] = data;
	UART_TxHead = tmp_head;
	UCSR0B |= (1<<UDRIE0);
}

//saving received data from hardware buffer to program buffer stored in RAM
ISR(USART_RX_vect){
	lcd_str("getin data...");
	_delay_ms(1000);
	uint8_t tmp_head;
	char data;
	data = UDR0;// get data from UART buffer
	// new head index value; if RxHead -> 31+1=32 -> 32 & 31 bitwise gives 0, which resets our index(head)
	tmp_head = ( UART_RxHead + 1 ) & UART_RX_BUFF_MASK;
	if ( tmp_head == UART_RxTail ){
		// TODO:  handle somehow that occurance
		// like signal this error with turning on LED
	} else{
		UART_RxHead = tmp_head;
		UART_RxBuff[tmp_head] = data;
	}
}


ISR(USART_UDRE_vect){
	if(UART_TxHead != UART_TxTail ){
		UART_TxTail = (UART_TxTail + 1) & UART_TX_BUFF_MASK;
		UDR0 = UART_TxBuff[UART_TxTail];
	}else {
		UCSR0B &= ~(1<<UDRIE0);
	}
}
