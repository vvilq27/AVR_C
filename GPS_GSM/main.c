
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
#include "common.h"
//#include <avr/pgmspace.h>
//#include <avr/eeprom.h>

#define __UBRR ((F_CPU+UART_BAUD*8UL) / (16UL*UART_BAUD)-1)
volatile uint8_t enable;
volatile uint8_t packet_tail;

//wrzuc do common


volatile GPS_frame gps;


// numer 0x61 lub 97 to a wg tabeli charseti ascii tez
//softuart: pc5 to RX a pc4 to TX
//note about struct: if field is too small it will display on lcd chosen field and the one after that
int main(){
	DDRB |= 0x01;		//singalisation
	DDRD |= 0x80;		//signalisation
	sei();
	USART_Init(__UBRR);
	nokia_lcd_init();
	nokia_lcd_clear();
	nokia_lcd_set_cursor(0, 0);
//	nokia_lcd_write_char(97,1); //writes "a" on screen
	softuart_init();
	softuart_turn_rx_on(); /* redundant - on by default */

/*
	struct thing{
		char name[10];
		char sname[10];
	}person = {"jack", "foo"};
	struct thing * ptr = &person;
	*/


	uint8_t  screen_pointer = 0;
//	char string[10]="";
//	char string2[10]="";
//	enable = 1; //usart.c rx vect changes it
//

	const char s[2] = ",";
	char sentence[80];

//	 get the first token
//	token = strtok(str, s);
//	while(token!=0){
//		nokia_lcd_write_string(token,1);
//		nokia_lcd_render();
//		nokia_lcd_set_cursor(0, (screen_pointer+=8));// taki pointer dziala
//		token = strtok(0, s);
//		_delay_ms(1000);
//	}


//	nokia_lcd_write_string("  GPS    START", 2);
//	nokia_lcd_render();
//	_delay_ms(1000);

//char GPS_frame_data[70];

//w usart rx mozna wrzucic sprawdzanie jaki typ ramki wchodzi za pomoca strcmp
// chce miec GPS_frame_data zeby w tym sprawdzac rodzaj ramki
	char temp_c;

	char cos[17] = "hans,,mad,klos";
	char alfa[5] ;
	char beta[10];
	char gama[10];

	strcpy(alfa,strtok(cos, s));
	strcpy(beta,strtok(0, s));
	strcpy(gama,strtok(0, s));

	nokia_lcd_write_string(alfa,1);
	nokia_lcd_set_cursor(0, (screen_pointer+=8));
	nokia_lcd_write_string(beta,1);
	nokia_lcd_set_cursor(0, (screen_pointer+=8));
	nokia_lcd_write_string(gama,1);
	nokia_lcd_render();

	while(1){
		PORTB ^= 1;
		_delay_ms(100);
//		char str[80] = "GPRMC,094155.00,A,5213.24298,N,02100.49863,E,0.149,angle,241017,,,A";
//		char str2[80] = "GPRMC,194154.00,A,9215.24298,N,12100.49863,E,0.12,angle,241017,,,A";

//		_delay_ms(1000);
//		strcpy(gps.type,strtok(str, s));
//			strcpy(gps.time,strtok(0, s));
//			strcpy(gps.foo,strtok(0, s));
//			strcpy(gps.lat,strtok(0, s));
//			strcpy(gps.ns,strtok(0, s));
//			strcpy(gps.lon,strtok(0, s));
//			strcpy(gps.we,strtok(0, s));
//
//			nokia_lcd_clear();
//			nokia_lcd_set_cursor(0, 0);
//			screen_pointer = 0;
//			nokia_lcd_write_string(gps.type,1);
//			nokia_lcd_set_cursor(0, (screen_pointer+=8));
//			nokia_lcd_write_string(gps.time,1);
//			nokia_lcd_set_cursor(0, (screen_pointer+=8));
//			nokia_lcd_write_string(gps.lat,1);
//			nokia_lcd_write_string(gps.ns,1);
//			nokia_lcd_set_cursor(0, (screen_pointer+=8));
//			nokia_lcd_write_string(gps.lon,1);
//			nokia_lcd_write_string(gps.we,1);
//
//			nokia_lcd_render();


			//test structure
//			if(change){
//				strcpy(ptr->name,"lolo");
//				strcpy(ptr->sname,"solo");
//			}else {
//				strcpy(ptr->name,"bolo");
//				strcpy(ptr->sname,"pikolo");
//			}
//			change ^= 1;
	/*	if(enable){
			PORTB |= 0x01;

			nokia_lcd_clear();
			nokia_lcd_set_cursor(0, 0);

			//write GPRMC
//			for(uint8_t i = 0; i < 5; i++)
//				nokia_lcd_write_char(frame_type[i], 1);
//
//			nokia_lcd_set_cursor(0,8);

			//do przerwania rx    <<<<<<<<<<<<<<<<<<<<<
			uint8_t i = 0;
			while(UART_RxTail != UART_RxHead){
				temp_c = uart_get_char();
					sentence[i] = temp_c;
					++i;
			}

			softuart_puts(sentence);
			softuart_puts("\r\n");

			strcpy(gps.time,strtok(sentence, s));
			strcpy(gps.foo,strtok(0, s));
			strcpy(gps.lat,strtok(0, s));
//			strcpy(gps.ns,strtok(0, s));
//			strcpy(gps.lon,strtok(0, s));
//			strcpy(gps.we,strtok(0, s));
//			strcpy(gps.speed,strtok(0, s));
//			strcpy(gps.angle,strtok(0, s));
//			strcpy(gps.date,strtok(0, s));


			strcpy(alfa,strtok(cos, s));
			strcpy(beta,strtok(0, s));
			nokia_lcd_write_string(alfa,1);
			nokia_lcd_set_cursor(0, (screen_pointer+=8));
			nokia_lcd_write_string(beta,1);

			screen_pointer = 0;
//			nokia_lcd_write_string(gps.time,1);
//			nokia_lcd_set_cursor(0, (screen_pointer+=8));
//			nokia_lcd_write_string(gps.lat,1);
//			nokia_lcd_write_string(" ",1);
//			nokia_lcd_write_string(gps.ns,1);
//			nokia_lcd_set_cursor(0, (screen_pointer+=8));
//			nokia_lcd_write_string(gps.lon,1);
//			nokia_lcd_write_string(" ",1);
//			nokia_lcd_write_string(gps.we,1);
//			nokia_lcd_set_cursor(0, (screen_pointer+=8));
//			nokia_lcd_write_string(gps.speed,1);
//			nokia_lcd_set_cursor(0, (screen_pointer+=8));
//			if(gps.angle != 0)
//			nokia_lcd_write_string(gps.angle,1);
//			else
//				nokia_lcd_write_string("0",1);
//			nokia_lcd_set_cursor(0, (screen_pointer+=8));
//			nokia_lcd_write_string(gps.date,1);

			nokia_lcd_render();

			softuart_puts(sentence);
			softuart_puts("\r\n");



			//write GPRMC sentence
//			while(UART_RxHead != UART_RxTail)
//				nokia_lcd_write_char(uart_get_char(), 1);

			nokia_lcd_render();

			enable = 0;
			PORTB &= ~0x01;
		}	// end of if		*/

	} 	// end of while loop
}	// end of main
