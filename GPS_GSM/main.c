// GPS GSM main
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "headers/softuart.h"
#include "headers/USART.h"
#include "headers/nokia5110.h"
#include "common.h"


volatile uint16_t timer1, timer2;

//tcpdump -i lo udp port 8125 -vv -X
int main(){
	//enable PD7 % PB0 pins for led flags
	DDRB |= 0x01;		//singalisation
	DDRD |= 0x80;		//signalisation gsm update

	USART_Init(__UBRR);

	PORTB |= (1<<PB0);

	uart_put_str("Starting\r\n");

//	timer2 = 25;
	timer1_init();
	sei();

//	while(timer2){
//		if(!timer2)
//			gsm_init();
//	}

	timer1 = 200;
	timer2 = 300;

	//check if frame is ok - 57 chars, to send it to server
	//======================================
	//				Main Loop
	//======================================
	while(1){
		if(!timer1){
			timer1 = 600;
			//reset gsm module
			gsm_init();
		}

		if(!timer2){
			UCSR0B |=  (1<<RXCIE0);	//enable rx, now we can listen for gps data
			sentence_field_cnt = 0;
			while(!frame_rcv_flag);	//wait for gps data

			PORTB |= (1<<PB0);
			gps_parse();
			//check if enough data to send to server
			if(sentenceCharCnt > 50)
				gsm_update();

			//delete
//			uart_put_str("\r\n");

			frame_rcv_flag = 0;
			UCSR0B &= ~ (1<<RXCIE0);	//disable rx
			PORTB &= ~(1<<PB0);
			timer2 = 150;			//inteval setup
		}

	} 	// end of while loop

}	// end of main
