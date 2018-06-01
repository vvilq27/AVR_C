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

//$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
//$GPRMC,123519,A,,,,,,,230394,,W*6A
void gps_parse(void){
	uint8_t i = 0;	//reset sentence index
	uint8_t gpsDelimiterCnt = 0;
	char temp_c;	//tmp char holder to copy whole sentence
	//flags goes like: 8.date 7.angle 6.speed 5.we 4.lat 3.ns 2.lon 1.time
	uint8_t structFlags = 0;

	//scanning gps rx buffer to find empty fields and put there space, NOT USED, sending raw buffer now
	while(UART_RxTail != UART_RxHead){
		temp_c = uart_get_char();
		if(temp_c == ',' && sentence[i-1]==','){

			switch(++gpsDelimiterCnt){
				case 1:						//time
					strcpy(gps.time,"      ");
					structFlags |= (1<<0);
					break;

				case 3:						//lat
					strcpy(gps.lat,"        ");
					structFlags |= (1<<1);
					break;

				case 4:						//ns
					strcpy(gps.ns," ");
					structFlags |= (1<<2);
					break;

				case 5:						//lon
					strcpy(gps.lon,"         ");
					break;

				case 6:						//we
					strcpy(gps.we," ");
					break;

				case 7:						//speed
					strcpy(gps.speed,"     ");
					break;

				case 8:						//angle
					strcpy(gps.angle,"     ");
					break;

				case 9:						//date
					strcpy(gps.date,"      ");
					break;

			}//end switch
			sentence[i++] = 32;		//put space
			sentence[i++] = temp_c;	//put coma from gps sentence
			continue;
		}
		sentence[i++] = temp_c;
	}
	uart_put_str("printing rawe sentence: \r\n");
	uart_put_str(sentence);
	uart_put_str("\r\n");

	//parse and put GPS data in struct
	//after parsing theres date left, it cuts all parts before
	if(!(structFlags & (1<<0)))
		strcpy(gps.time,strtok(sentence, s));
	uart_put_str(gps.time);
	strcpy(gps.foo,strtok(0, s));
	if(!(structFlags & (1<<1)))
		strcpy(gps.lat,strtok(0, s));
	if(!(structFlags & (1<<2)))
		strcpy(gps.ns,strtok(0, s));
	if(!(structFlags & (1<<3)))
		strcpy(gps.lon,strtok(0, s));
	if(!(structFlags & (1<<4)))
		strcpy(gps.we,strtok(0, s));
	if(!(structFlags & (1<<5)))
		strcpy(gps.speed,strtok(0, s));
	if(!(structFlags & (1<<6)))
		strcpy(gps.angle,strtok(0, s));
	if(!(structFlags & (1<<7)))
		strcpy(gps.date,strtok(0, s));

}

