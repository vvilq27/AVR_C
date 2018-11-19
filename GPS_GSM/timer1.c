/*
 * timer1.c
 *
 *  Created on: Dec 8, 2017
 *      Author: root
 */
#include <avr/io.h>
#include <avr/interrupt.h>		//isr definition
#include "common.h"

#define TIM1_VALUE 	63974

extern volatile uint16_t timer1, timer2;

void timer1_init(void){
	TCCR1B |= 	(1<<CS11) | (1<<CS10);	//prescaler 64
	TIMSK1 |=	(1<<TOIE1);	//OVF inter enable

	//set counter value for needed period
	TCNT1 =	63974;	//interupt 100ms for 1mhz &64 presc
}

ISR(TIMER1_OVF_vect){
	uint16_t cnt;

	cnt = timer1;
	if(cnt)
		timer1 = --cnt;

	cnt = timer2;
	if(cnt)
		timer2 = --cnt;

	//set counter value for needed period
	TCNT1 =	63974;

	if(!timer1){
		timer1 = 10;
		//reset gsm module
		uart_put_str("blink");
//		uart_put_int(99);	// delete this after
//			gsm_init();
	}
}
