/*
 * gps.c
 *
 *  Created on: Dec 8, 2017
 *      Author: root
 */
#include <avr/io.h>
#include <stdlib.h>
#include "headers/USART.h"
#include "common.h"


const char s[2] = ",";

//$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
//$GPRMC,123519,A,,,,,,,230394,,W*6A
//$GPRMC,123519,A,3,4,5,6,7,8,230394,,W*6A
//,123519,A, , , , , , ,230394, ,W
//,123519,A,,,,,,,230394,,W   this is in uart buff
void gps_parse(void){
	uint8_t i = 0;	//reset sentence index
	uint8_t gpsDelimiterCnt = 0;
	char bf[8];
	char temp_c;	//tmp char holder to copy whole sentence
	//flags goes like: 8.date 7.angle 6.speed 5.we 4.lat 3.ns 2.lon 1.time
	uint8_t structFlags = 0;

	//scanning gps rx buffer to find empty fields and put there space, NOT USED, sending raw buffer now
	while(UART_RxTail != UART_RxHead){
		temp_c = uart_get_char();
//		uart_put_char(temp_c); test
		if(temp_c == ',' && sentence[i-1]==','){

			switch(++gpsDelimiterCnt){
				case 1:						//time
					structFlags |= (1<<0);
					break;

				case 3:						//lat
					structFlags |= (1<<1);
					break;

				case 4:						//ns
					structFlags |= (1<<2);
					break;

				case 5:						//lon
					structFlags |= (1<<3);
					break;

				case 6:						//we
					structFlags |= (1<<4);
					break;

				case 7:						//speed
					structFlags |= (1<<5);
					break;

				case 8:						//angle
					structFlags |= (1<<6);
					break;

				case 9:						//date
					structFlags |= (1<<7);
					break;

			}//end switch
			//separate comas with space
			sentence[i++] = 32;		//put space
			sentence[i++] = temp_c;	//put coma from gps sentence
			continue;
		}//end if commas next to each other
		//copy char like they go
		sentence[i++] = temp_c;
	}
	uart_put_str("strct flag balues:  ");
	uart_put_str(itoa(structFlags,bf,10));
	uart_put_str("\r\n");

	uart_put_str("printing rawe sentence: \r\n");
	uart_put_str(sentence);
	uart_put_str("\r\n");

	//parse and put GPS data in struct
	//after parsing theres date left, it cuts all parts before
	if(structFlags & (1<<0)){
		strcpy(gps.time,"      ");
		uart_put_str("no time \r\n");
	}else
		strcpy(gps.time,strtok(sentence, s));

	strcpy(gps.foo,strtok(0, s));

	if(structFlags & (1<<1)){
		strcpy(gps.lat,"        ");
		uart_put_str("no lat \r\n");
	}else{
		strcpy(gps.lat,strtok(0, s));
	}

	if(structFlags & (1<<2)){
		strcpy(gps.ns," ");
		uart_put_str("no ns \r\n");

	}else{
		strcpy(gps.ns,strtok(0, s));
	}

	if(structFlags & (1<<3)){
		strcpy(gps.lon,"         ");
		uart_put_str("no lon \r\n");
	}else{
		strcpy(gps.lon,strtok(0, s));
	}

	if(structFlags & (1<<4)){
		uart_put_str("no we \r\n");
		strcpy(gps.we," ");
	}else
		strcpy(gps.we,strtok(0, s));

	if(structFlags & (1<<5)){
		uart_put_str("no speed \r\n");
		strcpy(gps.speed,"     ");
	}
	else
		strcpy(gps.speed,strtok(0, s));

	if(structFlags & (1<<6)){
		strcpy(gps.angle,"     ");
		uart_put_str("no angle\r\n");
	}
	else
		strcpy(gps.angle,strtok(0, s));

	if(structFlags & (1<<7)){
		strcpy(gps.date,"date12");
		uart_put_str("no date \r\n");
	}else
		strcpy(gps.date,strtok(0, s));

}

