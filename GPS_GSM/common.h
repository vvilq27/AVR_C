#ifndef COMMON_H_
#define COMMON_H_

#define F_CPU 1000000UL
#define __UBRR ((F_CPU+UART_BAUD*8UL) / (16UL*UART_BAUD)-1)	//not used


volatile uint8_t sentence_collected;			//flag indicates that all data collected from GPRMC sntnc
volatile char sentence[64];
volatile uint8_t sentenceCharCnt;		//used for gsm cipsend cmnd
volatile uint8_t sizeOfSentence;
volatile uint8_t sentence_field_cnt;	//flag used to count commas, and discard data after date field in GPRMC sntnc

#endif /* COMMON_H_ */
