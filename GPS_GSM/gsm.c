/*
 * gsm.c
 *
 *  Created on: Dec 5, 2017
 *      Author: root
 */

#include<util/delay.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include "headers/softuart.h"
#include "common.h"


extern volatile GPS_frame gps;
	//gsm start/ init GPRS
void gsm_init(void){
	softuart_puts_P( "at+sapbr=3,1,\"APN\",\"Internet\"\r\n" );
	_delay_ms(50);
	softuart_puts_P( "AT+SAPBR=1,1\r\n" );
	_delay_ms(500);
	softuart_puts_P( "AT+HTTPINIT\r\n" );
	_delay_ms(100);
//	softuart_puts_P( "AT+HTTPPARA=\"CID\",1\r\n" );
//	_delay_ms(1000);


}

//chyba wleci do timera
void gsm_update(void){
	sprintf(sentence, "at+httppara=\"URL\",\"http://145.239.82.215/gsm.php?tm=%s&lon=%s&ns=%s&lat=%s&we=%s&spd=%s&ang=%s&dt=%s\"\r\n", gps.time,gps.lon, gps.ns, gps.lat, gps.we, gps.speed, gps.angle, gps.date);
	softuart_puts( sentence );
	_delay_ms(100);

	softuart_puts("at+httpaction=0\r\n");
	_delay_ms(350);

}
