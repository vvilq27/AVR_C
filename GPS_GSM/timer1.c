/*
 * timer1.c
 *
 *  Created on: Dec 8, 2017
 *      Author: root
 */
#include <avr/io.h>
#include <avr/interrupt.h>		//isr definition
#include "common.h"

extern volatile uint16_t timer1, timer2;

void timer1_init(void){
//	TCCR1A |=	0;
	TCCR1B |= 	(1<<CS12) | (1<<CS10);	//prescaler 1024
	TIMSK1 |=	(1<<TOIE1);	//inter enable
	TCNT1 =	64754;	//interupt 100ms for 8mhz &1024 presc

}

ISR(TIMER1_OVF_vect){
	uint16_t cnt;

	cnt = timer1;
	if(cnt) timer1 = --cnt;

	cnt = timer2;
	if(cnt) timer2 = --cnt;

	TCNT1 =	64754;
}
