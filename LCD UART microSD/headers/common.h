//macros common for all files
#include <avr/io.h>

#ifndef HEADERS_COMMON_H_
#define HEADERS_COMMON_H_

#define F_CPU 8000000UL
#define USE_LCD 1
#define USE_SERIAL_MONITOR 1
#define LED_OFF(x) (PORTD &= ~(1 << x))
#define LED_ON(x) (PORTD |= (1 << x))
#define TEST_LED_ON (PORTB|=(1<<PB1))

typedef uint8_t uint8;
typedef uint16_t uint16;


#endif /* HEADERS_COMMON_H_ */
