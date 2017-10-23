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
//#define __UBRR ((F_CPU) / (16UL*UART_BAUD)-1)
#define __UBRR ((F_CPU+UART_BAUD*8UL) / (16UL*UART_BAUD)-1)
#if U2SPEED == 1
	#define __UBRR ((F_CPU+UART_BAUD*4UL) / (8UL*UART_BAUD)-1))
#endif
#define UART_RX_BUFF_SIZE 32
#define UART_RX_BUFF_MASK (UART_RX_BUFF_SIZE - 1)
#define UART_TX_BUFF_SIZE 16
#define UART_TX_BUFF_MASK (UART_TX_BUFF_SIZE - 1)


volatile char UART_RxBuff[UART_RX_BUFF_SIZE]; // buffer definition
volatile char UART_RxData[18]; // buffer definition
volatile uint8_t UART_RxHead; // buffer data end index
volatile uint8_t UART_RxTail; // buffer data beginnig index
volatile char UART_TxBuff[UART_TX_BUFF_SIZE];
volatile uint8_t UART_TxHead;
volatile uint8_t UART_TxTail;
volatile uint8_t data_index;


void uart_put_char(char );
char* uart_get_char(void);
void USART_Init(unsigned int );
void USART_Init_check(void);
void uart_put_char(char );
void uart_put_str(char*);
#endif /* USART_H_ */
