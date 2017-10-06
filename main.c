/*
 * main.c
 *
 *  Created on: 23.09.2017
 *      Author: arazu
 */
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "headers/nokia5110.h"

int main(){
	DDRB |= 0x01;
	nokia_lcd_init();
	nokia_lcd_clear();
	nokia_lcd_set_cursor(0, 0);
	nokia_lcd_render();

	uint8_t count = 0;
	char string[4]="";
	uint8_t tab[3]= {4,5,6};
	uint8_t tab2[3] = {2,2,2}; // 125 ma adres 2288

	volatile unsigned int *myPointer4 = (volatile unsigned int *)0x8f0;

	/* 	sprawdzanie adresow zmiennych
	nokia_lcd_set_cursor(0,0);
	sprintf(string, "%d ",&tab[0]);	//zmien 1 adres 1
	nokia_lcd_write_string(string,1);
	sprintf(string, "%d ",&tab[2]);	//zmien 1 adres 3
	nokia_lcd_write_string(string,1);
	sprintf(string, "%d ",&tab2[0]);	//zmien 2 adres 1
	nokia_lcd_write_string(string,1);
	nokia_lcd_set_cursor(0,10);
	sprintf(string, "%d ",&string[0]);	//zmien 2 adres 1
	nokia_lcd_write_string(string,1);
	sprintf(string, "%d ",&string[5]);	//zmien 2 adres 1
	nokia_lcd_write_string(string,1);
	*/

	while(1){
		PORTB ^= 0x01;
		_delay_ms(3000);

		nokia_lcd_clear();
		nokia_lcd_set_cursor(0, 0);
		sprintf(string, "%d -> ",&tab2[0]);	//adres 1 komorki
		nokia_lcd_write_string(string,1);
		for(char i =0; i<4; i++){string [i]= "";}
		sprintf(string, "%x",(tab2[0]));	// wartosc 1 komorki tablicy
		nokia_lcd_write_string(string,1);

		for(char i =0; i<4; i++){string [i]= "";}
		// odczyt za pomoca wskaznika
		nokia_lcd_set_cursor(0,10);
		sprintf(string, "%d -> ",&tab2[1]);	//adres na ktory wskazujewskaznik
		nokia_lcd_write_string(string,1);
		for(char i =0; i<4; i++){string [i]= "";}
		sprintf(string, "%x",tab2[1]);	//wartosc z adresu pamieci 1 komorki tablicy
		nokia_lcd_write_string(string,1);

		for(char i =0; i<4; i++){string [i]= "";}
		nokia_lcd_set_cursor(0,20);
		sprintf(string, "%d -> ",&tab2[2]);	//adres na ktory wskazujewskaznik
		nokia_lcd_write_string(string,1);
		for(char i =0; i<4; i++){string [i]= "";}
		sprintf(string, "%x",tab2[2]);	//wartosc z adresu pamieci 1 komorki tablicy
		nokia_lcd_write_string(string,1);

		for(char i =0; i<4; i++){string [i]= "";}
		nokia_lcd_set_cursor(0,30);
		sprintf(string, "%d -> ",&tab[0]);	//adres na ktory wskazujewskaznik
		nokia_lcd_write_string(string,1);
		for(char i =0; i<4; i++){string [i]= "";}
		sprintf(string, "%x",tab[0]);	//wartosc z adresu pamieci 1 komorki tablicy
		nokia_lcd_write_string(string,1);

		for(char i =0; i<4; i++){string [i]= "";}
		nokia_lcd_set_cursor(0,40);
		sprintf(string, "%d->",myPointer4);	//adres na ktory wskazujewskaznik
		nokia_lcd_write_string(string,1);
		for(char i =0; i<4; i++){string [i]= "";}
		sprintf(string, "%x",*myPointer4);	//wartosc z adresu pamieci 1 komorki tablicy
		nokia_lcd_write_string(string,1);
		nokia_lcd_render();

		myPointer4++;
		if(myPointer4 > 2292) {myPointer4 = 2288;}

	}
}

// stary kod

/*
nokia_lcd_set_cursor(10, 0);
		nokia_lcd_write_string("BANSUJ",2);
		nokia_lcd_render();
		nokia_lcd_clear();
		_delay_ms(350);
		nokia_lcd_set_cursor(8, 25);
		nokia_lcd_write_string("DZIWKO!",2);
		nokia_lcd_render();
		nokia_lcd_clear();
*/


