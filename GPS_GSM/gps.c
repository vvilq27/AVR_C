#include <avr/io.h>
#include <stdlib.h>
#include "headers/USART.h"
#include "common.h"

//delete?
const char s[2] = ",";

//$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
//$GPRMC,123519,A,,,,,,,230394,,W*6A

void gps_parse(void){
	uint8_t i = 0;	//reset sentence index
	sentenceCharCnt = 0;

	while(UART_RxTail != UART_RxHead){
		sentence[i++] = uart_get_char();
		sentenceCharCnt++;
	}
}

