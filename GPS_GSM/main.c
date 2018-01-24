// GPS GSM main

//#include <avr/pgmspace.h>
//#include <avr/eeprom.h>
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


// numer 0x61 lub 97 to a wg tabeli charset(nokia) i ascii tez, takze charset==ascii
//softuart: pc5 to RX a pc4 to TX
//note about struct: if field is too small it will display on lcd chosen field and the one after that
//softuart uses TIMER0
//itoa na zmiennej timmer powodowalo dziwne dzialanie programu, byc moze juz konczy sie ram
volatile uint16_t timer1, timer2;

int main(){
	DDRB |= 0x01;		//singalisation
	DDRD |= 0x80;		//signalisation

	sei();
	USART_Init(__UBRR);
	nokia_lcd_init();
	softuart_init();
	softuart_turn_rx_on();  //redundant - on by default
	timer1_init();

	nokia_lcd_clear();
	nokia_lcd_set_cursor(0, 0);

	//main variables
	uint8_t  screen_pointer = 0;
	timer1 = 30;
	timer2 = 100;

	nokia_lcd_write_string("  GPS    START", 2);
	nokia_lcd_render();
	_delay_ms(1000);


	//======================================
	//				Main Loop
	//======================================
	while(1){

		if(!timer1){
			timer1 = 580;
			//reset gsm module and lcd
			gsm_init();
			nokia_lcd_init();
		}

		if(!timer2){
			timer2 = 150;
			gsm_update();
			PORTD ^= (1<<PD7);
		}

		//usart parser after finding '*' in RMC sets enable to 1
		if(enable){
			nokia_lcd_clear();
			nokia_lcd_set_cursor(0, 0);
			gps_parse();



			//======================================
			//				GPS PRINT
			//======================================
			screen_pointer = 0;
			//print gps data on nokia LCD - RMC sentence
			nokia_lcd_write_string(gps.time,1);
//			nokia_lcd_write_string(" ", 1);
//			nokia_lcd_write_string(timer_cnt, 1);
			nokia_lcd_set_cursor(0, (screen_pointer+=8));
			nokia_lcd_write_string(gps.lat,1);
			nokia_lcd_write_string(" ",1);
			nokia_lcd_write_string(gps.ns,1);
			nokia_lcd_set_cursor(0, (screen_pointer+=8));
			nokia_lcd_write_string(gps.lon,1);
			nokia_lcd_write_string(" ",1);
			nokia_lcd_write_string(gps.we,1);
			nokia_lcd_set_cursor(0, (screen_pointer+=8));
			nokia_lcd_write_string(gps.speed,1);
			nokia_lcd_set_cursor(0, (screen_pointer+=8));
			if(gps.angle != 0)
			nokia_lcd_write_string(gps.angle,1);
			else
				nokia_lcd_write_string("0",1);
			nokia_lcd_set_cursor(0, (screen_pointer+=8));
			nokia_lcd_write_string(gps.date,1);
			nokia_lcd_render();

			enable = 0;	//disable print feature in main loop
			PORTB &= ~(1<<PB0);	//it sets when '*' in usart comes
		}	// end of if

	} 	// end of while loop
}	// end of main
