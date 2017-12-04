/*
	AVR Software-Uart Demo-Application
	Version 0.4, 10/2010

	by Martin Thomas, Kaiserslautern, Germany
	<eversmith@heizung-thomas.de>
	http://www.siwawi.arubi.uni-kl.de/avr_projects
*/

/*
Test environment/settings:
- avr-gcc 4.3.3/avr-libc 1.6.7 (WinAVR 3/2010)
- Atmel ATmega324P @ 8MHz internal RC, ATtiny85 @ 1MHz internal RC
- 2400bps
*/

/*
AVR Memory Usage (-Os, no-inline small-functions, relax)
----------------
Device: atmega324p

Program:     926 bytes (2.8% Full)
(.text + .data + .bootloader)

Data:         52 bytes (2.5% Full)
(.data + .bss + .noinit)


AVR Memory Usage (-Os)
----------------
Device: attiny85

Program:     828 bytes (10.1% Full)
(.text + .data + .bootloader)

Data:         52 bytes (10.2% Full)
(.data + .bss + .noinit)

*/

#define WITH_STDIO_DEMO   0 /* 1: enable, 0: disable */
#define F_CPU 8000000UL

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "softuart.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>


//before compile, check mcu type and pin connections!
//int main(void){
//	DDRC |= _BV(0);
//	while(1){
//		PORTC ^= (1<<0);
//		_delay_ms(100);
//	}
//}

/*
 *
 * 				STANDARD IO STREAM
 *
 */
#if WITH_STDIO_DEMO
#include <stdio.h>

// interface between avr-libc stdio and software-UART
static int my_stdio_putchar( char c, FILE *stream )
{
	if ( c == '\n' ) {
		softuart_putchar( '\r' );
	}
	softuart_putchar( c );

	return 0;
}

FILE suart_stream = FDEV_SETUP_STREAM( my_stdio_putchar, NULL, _FDEV_SETUP_WRITE );

static void stdio_demo_func( void )
{
	stdout = &suart_stream;
	printf( "This output done with printf\n" );
	printf_P( PSTR("This output done with printf_P\n") );
}
#endif /* WITH_STDIO_DEMO */


//rx pd6 tx pd7
// set baud in softuart.h

int main(void)
{
	DDRC |= _BV(0);		//test purposes
//	static const char pstring[] PROGMEM =
//		"adapted for Atmel AVR and this demo by Martin Thomas\r\n";
	softuart_init();
	softuart_turn_rx_on(); /* redundant - on by default */

	sei();

//	softuart_puts_P( "\r\nSoftuart Demo-Application\r\n" );    // "implicit" PSTR
//	softuart_puts_p( PSTR("generic softuart driver code by Colin Gittins\r\n") ); // explicit PSTR
//	softuart_puts_p( pstring ); // pstring defined with PROGMEM
//	softuart_puts( "--\r\n" );  // string "from RAM"

/*	//gsm start/ init GPRS
	_delay_ms(10000);
	softuart_puts_P( "at+sapbr=3,1,\"APN\",\"Internet\"\r\n" );
	_delay_ms(1000);
	softuart_puts_P( "AT+SAPBR=1,1\r\n" );
	_delay_ms(2000);
	softuart_puts_P( "AT+HTTPINIT\r\n" );
	_delay_ms(2000);
//	softuart_puts_P( "AT+HTTPPARA=\"CID\",1\r\n" );
//	_delay_ms(1000);
	softuart_puts_P( "at+httppara=\"URL\",\"145.239.82.215/?lon=123&lat=321\"\r\n" );
	_delay_ms(1000);		*/


//	PORTC |= 1; // test
	for (;;) {
		softuart_puts_P( "AT+HTTPACTION=0\r\n" );
		_delay_ms(2500);

	}

	return 0; /* never reached */
}
