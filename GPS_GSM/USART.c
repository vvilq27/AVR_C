#include <avr/interrupt.h>
#include <avr/io.h>
#include "headers/USART.h"
#include "common.h"

// uses register names declared in iom16.h not iom328p.h like wtf? i've lost 2 days on figuring that out

void USART_Init(unsigned int baud){
	DDRD |= (1<<6);
	//set baud rate
//	UBRR0H = (uint8_t) (baud>>8);
	UBRR0L = baud;
	UCSR0A|=(1<<U2X0);//double speed aysnc
//	enable receiver and transmitter and TX/ RX INTERRUPT
	UCSR0B |= (1<<TXEN0)| (1<<RXEN0) ;// | (1<<TXCIE0); //rx enabled in main loop
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
		//PORTD |= (1<<PD7);
	}
	UART_TxBuff[tmp_head] = data;
	UART_TxHead = tmp_head;
	UCSR0B |= (1<<UDRIE0); //starts TX interrupt
}

void uart_put_str(const char* s){
	while(*s)
		uart_put_char(*s++);
}

//usart receive handling
//saving received data from hardware buffer to program buffer stored in RAM
ISR(USART_RX_vect){

	uint8_t tmp_head;
	char data;
	data = UDR0;// get data from UART buffer

	//got new sentence, reset parsing flags
	if(data == '$'){
		frame_type_char_count = 0;
		return;
	}
	//get frame type to check in next step if it is GPRMC
	if(frame_type_char_count < 6){
		frame_type[frame_type_char_count] = data;
		frame_type_char_count++;
		return;
	}
	//we got frame name now, time to check its type
	// if statement true when strings not equal, if equal we continue parsing
	if(strncmp(frame_type, "GPRMC,", 6)){
		return;
	}

	//collecting chars of GPRMC sentence
	if(sentence_collected != 1 && data != 0xa && data != 0xd ){ // if received /r or $  or sentence collected then ignore it
//		if ( tmp_head == UART_RxTail ){
//			// TODO:  handle somehow that occurance
//		}	// like signal this error with turning on LED

		if(data == 44)		//if comma
			sentence_field_cnt++;

		if(sentence_field_cnt >= 9 ){//ignore data after * in gps frame
			sentence_collected = 1;
			return;
		}

		// new head index value; if RxHead -> 31+1=32 -> 32 & 31 bitwise gives 0, which resets our index(head)
		tmp_head = ( UART_RxHead + 1 ) & UART_RX_BUFF_MASK;		//shifting rx buffer index
		UART_RxHead = tmp_head;									//shifting rx buffer index, may be as one liner
		UART_RxBuff[tmp_head] = data;							//insert char from rx hard buff to rx soft buff
	}
}

//disabled atm
//caused IC reset, duno why
//UDR0 empty interrupt
ISR(USART_UDRE_vect){
	if(UART_TxHead != UART_TxTail ){
		UART_TxTail = (UART_TxTail + 1) & UART_TX_BUFF_MASK;
		UDR0 = UART_TxBuff[UART_TxTail];
	}else {
		UCSR0B &= ~(1<<UDRIE0);
	}
}
