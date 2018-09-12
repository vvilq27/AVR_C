#include<util/delay.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include "headers/softuart.h"
#include "common.h"
#include "headers/USART.h"

//gsm start/ init UDP parameters
void gsm_init(void){
	uart_put_str( "AT+CGATT=1\r\n");
	_delay_ms(50);
	uart_put_str( "AT+CSTT=\"Internet\",\"\",\"\"\r\n" );
	_delay_ms(50);
	uart_put_str( "AT+CIICR\r\n");
	_delay_ms(150);
	uart_put_str( "AT+CIFSR\r\n");
	_delay_ms(50);
	uart_put_str( "AT+CIPSTART=\"UDP\",\"89.65.242.51\",9999\r\n" );
	_delay_ms(150);
}

//example string to send:
//,203141.00,A,5122.66485,N,02015.89398,E,0.704,,020618,,,AN
void gsm_update(void){
	char sendCommand[16];
	sprintf(sendCommand, "at+cipsend=%i\r\n", sentenceCharCnt);
	uart_put_str(sendCommand);
	_delay_ms(50);
	uart_put_str(sentence);
	uart_put_str("\r\n");
	//to delete
//	uart_put_str("\r\n");
//	for(int i = 0; i < sentenceCharCnt; i++)
//		uart_put_char(sentence[i]);

}
