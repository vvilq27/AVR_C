/*
 * gps.c
 *
 *  Created on: Dec 8, 2017
 *      Author: root
 */
#include <avr/io.h>
#include "headers/USART.h"
#include "common.h"


const char s[2] = ",";

void gps_parse(void){
	uint8_t i = 0;	//reset sentence index
	char temp_c;	//tmp char holder to copy whole sentence
	while(UART_RxTail != UART_RxHead){
		temp_c = uart_get_char();
		if(temp_c == ',' && sentence[i-1]==','){
			sentence[i] = 32;	//put space
			i++;
			sentence[i] = temp_c;
			i++;
			continue;
		}
		sentence[i] = temp_c;
		++i;
	}


	//parse and put GPS data in struct
	//after parsing theres date left, it cuts all parts before
	strcpy(gps.time,strtok(sentence, s));
	strcpy(gps.foo,strtok(0, s));
	strcpy(gps.lat,strtok(0, s));
	strcpy(gps.ns,strtok(0, s));
	strcpy(gps.lon,strtok(0, s));
	strcpy(gps.we,strtok(0, s));
	strcpy(gps.speed,strtok(0, s));
	strcpy(gps.angle,strtok(0, s));
	strcpy(gps.date,strtok(0, s));
}

