
//#include <avr/pgmspace.h>
//#include <avr/eeprom.h>

#define F_CPU 8000000UL

//#include <stdio.h>
//#include <util/delay.h>
//#include <inttypes.h>
//#include <stdio.h>
//#include <ctype.h>

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "headers/softuart.h"
#include "headers/USART.h"
#include "headers/nokia5110.h"
//#include <avr/pgmspace.h>
//#include <avr/eeprom.h>

#define __UBRR ((F_CPU+UART_BAUD*8UL) / (16UL*UART_BAUD)-1)
volatile uint8_t enable;
volatile uint8_t packet_tail;

// numer 0x61 lub 97 to a wg tabeli charseti ascii tez
//softuart: pc5 to RX a pc4 to TX
int main(){
	DDRB |= 0x01;
	DDRD |= 0x80;
	sei();
	USART_Init(__UBRR);
	nokia_lcd_init();
	nokia_lcd_clear();
	nokia_lcd_set_cursor(0, 0);
//	nokia_lcd_write_char(97,1); //writes "a" on screen
	softuart_init();
	softuart_turn_rx_on(); /* redundant - on by default */

//	typedef struct{
//		char type[6];
//		char time[10];
//		char foo[2];
//		char lat[12];
//		char ns[3];
//		char lon[12];
//		char we[3];
//		char speed[5];
//		char angle[5];
//		char date[6];
//	} GPS_frame;

//	GPS_frame gps;

//
//	uint8_t  screen_pointer = 0;
//	char string[10]="";
//	char string2[10]="";
//	enable = 1; //usart.c rx vect changes it
//
//	char str[80] = "GPRMC,094155.00,A,5213.24298,N,02100.49863,E,0.149,angle,241017,,,A";
//	char str2[80] = "$GPRMC,094155.00,A,5213.24298,N,02100.49863,E,0.149,angle,241017,,,A";
//	const char s[2] = ",";


	/* get the first token */		/*
	strcpy(gps.type,strtok(str, s));
	strcpy(gps.time,strtok(0, s));
	strcpy(gps.foo,strtok(0, s));
	strcpy(gps.lat,strtok(0, s));
	strcpy(gps.ns,strtok(0, s));
	strcpy(gps.lon,strtok(0, s));
	strcpy(gps.we,strtok(0, s));


	nokia_lcd_write_string(gps.type,1);
	nokia_lcd_set_cursor(0, (screen_pointer+=8));
	nokia_lcd_write_string(gps.time,1);
	nokia_lcd_set_cursor(0, (screen_pointer+=8));
	nokia_lcd_write_string(gps.lat,1);
	nokia_lcd_write_string(gps.ns,1);
	nokia_lcd_set_cursor(0, (screen_pointer+=8));
	nokia_lcd_write_string(gps.lon,1);
	nokia_lcd_write_string(gps.we,1);
	nokia_lcd_render();			*/

//	token = strtok(str, s);
//	while(token!=0){
//		nokia_lcd_write_string(token,1);
//		nokia_lcd_render();
//		nokia_lcd_set_cursor(0, (screen_pointer+=8));// taki pointer dziala
//		token = strtok(0, s);
//		_delay_ms(1000);
//	}

//	if(!strncmp(str2, "GPRMC", 5))
//		PORTB |= 1;
	nokia_lcd_write_string("  GPS    START", 2);
	nokia_lcd_render();
	_delay_ms(1000);

//char GPS_frame_data[70];

//w usart rx mozna wrzucic sprawdzanie jaki typ ramki wchodzi za pomoca strcmp
// chce miec GPS_frame_data zeby w tym sprawdzac rodzaj ramki

	while(1){
//			//zrob diode w timerze
		if(enable){
			PORTB |= 0x01;

			nokia_lcd_clear();
			nokia_lcd_set_cursor(0, 0);

//			uint8_t data_size =0;
			/*while(UART_RxHead != UART_RxTail){
				GPS_frame_data[data_size] = uart_get_char();
				data_size++;
			}
			for(uint8_t i = 0; i < data_size; i++){
				nokia_lcd_write_char(GPS_frame_data[i], 1);
//				uart_put_char(GPS_frame_data[i]);
			}*/
			for(uint8_t i = 0; i < 5; i++){
				nokia_lcd_write_char(frame_type[i], 1);

			}
			nokia_lcd_set_cursor(0,8);
			while(UART_RxHead != UART_RxTail){
				char c = uart_get_char();
				nokia_lcd_write_char(c, 1);
				softuart_putchar(c);
			}
			softuart_puts("\r\n");
			nokia_lcd_render();

			// test uart data buffer
			/*
			nokia_lcd_set_cursor(0,40);
			nokia_lcd_write_string(itoa(packet_tail, string,10),1);
			nokia_lcd_write_string("  ", 1);
			nokia_lcd_write_string(itoa(UART_RxHead, string,10),1);
			nokia_lcd_write_string("  ", 1);
			nokia_lcd_write_string(itoa(UART_RxTail , string,10),1);
			nokia_lcd_write_string("  ", 1);
			nokia_lcd_write_string(itoa(data_size , string,10),1);

										*/

			enable = 0;
			PORTB &= ~0x01;
		}

	}
}
