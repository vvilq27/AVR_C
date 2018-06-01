// GPS GSM main
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "headers/softuart.h"
#include "headers/USART.h"
#include "headers/nokia5110.h"
#include "common.h"


// numer 0x61 lub 97 to a wg tabeli charset(nokia) i ascii tez, takze charset==ascii
//softuart: pc5 to RX a pc4 to TX
//note about struct: if field is too small it will display on lcd chosen field and the one after that
//softuart uses TIMER0
//itoa na zmiennej timmer powodowalo dziwne dzialanie programu, byc moze juz konczy sie ram
volatile uint16_t timer1, timer2;

int main(){
	DDRB |= 0x01;		//singalisation
	DDRD |= 0x80;		//signalisation gsm update

	sei();
	USART_Init(__UBRR);
//	softuart_init();
//	softuart_turn_rx_on();  //redundant - on by default
	timer1_init();

	timer1 = 5;
	timer2 = 10;

	//======================================
	//				Main Loop
	//======================================
	//removing softuart, hardware uart will do everything
	//problem: jak mierzyc dlugosc wysylanego textu na serva przez cipsend
	//ew jak ustawic spacje w sentence, zeby ciagle miala taka sama dlugosc
	while(1){
		if(!timer1){
			timer1 = 50;
			//reset gsm module
//			gsm_init();
		}

		if(!timer2){
			gps_parse();
			gsm_update();
			timer2 = 5;

		}
	} 	// end of while loop
}	// end of main
