#include <avr/interrupt.h>
#include <avr/io.h>
#include "headers/USART.h"

// uses register names declared in iom16.h not iom328p.h like wtf? i've lost 2 days on figuring that out

void USART_Init(unsigned int baud){
	DDRD |= (1<<6);
	//set baud rate
	UBRR0H = (uint8_t) (baud>>8);
	UBRR0L = (uint8_t) (baud);
//	enable receiver and transmitter and RX INTERRUPT
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0)|(1<<RXCIE0);
//	set frame format: 8 data bits, 1 stop bits, no parity
	UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
}

//reading received and saved data stored in program buffer
char uart_get_char(void){
	if( UART_RxHead == UART_RxTail )
		return 0;// check if indexes are equal => no data in buffer
	else
		UART_RxTail = (UART_RxTail + 1) & UART_RX_BUFF_MASK;//shifts read register

	return UART_RxBuff[UART_RxTail];//output received data
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
	uint8_t tmp_head;
	char data;
	data = UDR0;// get data from UART buffer

	if(data == '$'){
		packet_tail = 0;
		frame_type_char_count = 0;
		return;
	}
	//get frame type to pick only GPRMC
	if(frame_type_char_count < 5){
		frame_type[frame_type_char_count] = data;
		frame_type_char_count++;
		return;
	}
	// if statement true when strings not equal
	if(strncmp(frame_type, "GPRMC", 5)){
		return;
	}

	if(packet_tail != 1 && data != 0xa && data != 0xd ){ // if received /r or $ then ignore it
//		if ( tmp_head == UART_RxTail ){
//			// TODO:  handle somehow that occurance
//		}	// like signal this error with turning on LED

		if(data == '*' ){//ignore data after * in gps frame
			packet_tail = 1;
			enable = 1;	//enable main loop code
			return;
		}
		// new head index value; if RxHead -> 31+1=32 -> 32 & 31 bitwise gives 0, which resets our index(head)
		tmp_head = ( UART_RxHead + 1 ) & UART_RX_BUFF_MASK;
		UART_RxHead = tmp_head;
		UART_RxBuff[tmp_head] = data;
	}
}

//UDR0 empty interrupt
ISR(USART_UDRE_vect){

	if(UART_TxHead != UART_TxTail ){
		UART_TxTail = (UART_TxTail + 1) & UART_TX_BUFF_MASK;
		UDR0 = UART_TxBuff[UART_TxTail];
	}else {
		UCSR0B &= ~(1<<UDRIE0);
	}
}
