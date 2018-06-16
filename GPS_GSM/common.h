#ifndef COMMON_H_
#define COMMON_H_

#define F_CPU 8000000UL
#define __UBRR ((F_CPU+UART_BAUD*8UL) / (16UL*UART_BAUD)-1)

//prob can be done smaller
//typedef struct{
//	char type[6];
//	char time[10];
//	char foo[2];
//	char lat[12];
//	char ns[3];
//	char lon[12];
//	char we[3];
//	char speed[6];
//	char angle[7];
//	char date[7];
//} GPS_frame;

volatile uint8_t enable;
volatile uint8_t packet_tail;
volatile char sentence[64];
volatile uint8_t sentenceCharCnt;
//volatile GPS_frame gps;
volatile uint8_t sizeOfSentence;
volatile uint8_t frame_rcv_flag;

#endif /* COMMON_H_ */
