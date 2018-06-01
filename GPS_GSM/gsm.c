#include<util/delay.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include "headers/softuart.h"
#include "common.h"
#include "headers/USART.h"


extern volatile GPS_frame gps;
	//gsm start/ init UDP parameters
void gsm_init(void){
	uart_put_str( "AT+CGATT=1\r\n");
	_delay_ms(50);
	uart_put_str( "AT+CSTT=\"Internet\",\"\",\"\"\r\n" );
	_delay_ms(50);
	uart_put_str( "at+cifsr\r\n");
	_delay_ms(50);
	uart_put_str( "AT+CIPSTART=\"UDP\",\"89.65.242.51\",9999\r\n" );
	_delay_ms(50);
}

void gsm_update(void){
//	sprintf(sentence, "at+httppara=\"URL\",\"http://89.65.242.51/gsm.php?tm=%s&lon=%s&ns=%s&lat=%s&we=%s&spd=%s&ang=%s&dt=%s\"\r\n", gps.time,gps.lon, gps.ns, gps.lat, gps.we, gps.speed, gps.angle, gps.date);
	char sendCommand[16];
	sprintf(sentence, "%s,%s,%s,%s,%s,%s,%s,%s\r\n", gps.time,gps.lat, gps.ns, gps.lon, gps.we, gps.speed, gps.angle, gps.date);
	sprintf(sendCommand, "at+cipsend=%i\r\n", sizeof(sentence));
//	uart_put_str(sendCommand);
	uart_put_str("\r\n");
	uart_put_str("formated sentence gsm update: \r\n");
	uart_put_str(sentence);
	uart_put_str("\r\n");

	_delay_ms(100);

//	uart_put_str("at+httpaction=0\r\n");
//	_delay_ms(350);

}
