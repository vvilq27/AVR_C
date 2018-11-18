/*
 * UART.h
 *
 *  Created on: Oct 11, 2017
 *      Author: root
 */

#include <avr/io.h>

#ifndef USART_H_
#define USART_H_

#define UART_BAUD 9600
//#define __UBRR ((F_CPU+UART_BAUD*8UL) / (16UL*UART_BAUD)-1))
#if U2SPEED == 1
	#define __UBRR ((F_CPU+UART_BAUD*4UL) / (8UL*UART_BAUD)-1))
#endif
//BUFFER SIZE MUST BE 2/4/8/16/32... because of the way Rx_head is reloaded to 0
//if we want any number we got to change this mechanism in rx ISR
//like check overflow in ifs
#define UART_RX_BUFF_SIZE 128
#define UART_RX_BUFF_MASK (UART_RX_BUFF_SIZE - 1)
#define UART_TX_BUFF_SIZE 128
#define UART_TX_BUFF_MASK (UART_TX_BUFF_SIZE - 1)


volatile char UART_RxBuff[UART_RX_BUFF_SIZE]; // buffer definition
volatile uint8_t UART_RxHead; // buffer data end index
volatile uint8_t UART_RxTail; // buffer data beginnig index
volatile char UART_TxBuff[UART_TX_BUFF_SIZE];
volatile uint8_t UART_TxHead;
volatile uint8_t UART_TxTail;
extern volatile uint8_t enable;
extern volatile uint8_t sentence_collected;
volatile char frame_type[6];
volatile uint8_t frame_type_char_count;
extern volatile uint8_t frame_rcv_flag;

void uart_put_char(char );

char uart_get_char(void);
void USART_Init(unsigned int);



#endif /* USART_H_ */
