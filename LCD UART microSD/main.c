//SD CARD
//UART <-> PC		SPI <-> SD CARD
#include "headers/common.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "headers/LCD.h"
#include "headers/USART.h"
#include "headers/SPI.h"
#include "headers/sd/mmc_if.h"
#include "headers/sd/tff.h"

FATFS fatfs;									//variable handling SDcard
FIL logfile;									//variable handling file that is being used

//red diode(PD2) indicates something went wrong
int main(void){
	DDRB |= (1<<PD1);							// test led output
	DDRD |= (1<<PD2); 							//sd status led pin out
	sei();										//global interrupt enable
	SPI_init();
	USART_Init(__UBRR);
	lcd_init();
	lcd_cls();

	uint8_t write_result;
	int i;
	FRESULT res;
	uint8_t data_to_write[] = "test 123";
	uint8_t bytes_read[16];
	char filename[] = "test.txt";
#if USE_SERIAL_MONITOR == 1
	//setup uart as standard output, now printf formats text and sends it via uart
	FILE console_display = FDEV_SETUP_STREAM(uart_put_char, NULL, _FDEV_SETUP_RW);
	stdout=&console_display;
	//console - say hello
	printf("\n\r\n\r**  mmc_demo is alive. **\n\r");
#endif
	//start signalisation
	LED_ON(2);
	_delay_ms(800);
	LED_OFF(2);
	_delay_ms(800);
	LED_ON(2);
	_delay_ms(800);
	LED_OFF(2);
	//=====================================================
	//				drive initialization
	//=====================================================
	i = mmc_init();
	if (i){//i=1 card not detected; i=2 failure
		LED_ON(2);								//theres problem
#if USE_LCD == 1
		lcd_locate(1,0);
		lcd_int(i);								//display error number
#endif
	}
	//=====================================================
	//					mounting drive
	//=====================================================
	if (f_mount(0, &fatfs)){
		LED_ON(2);
#if USE_SERIAL_MONITOR == 1
		printf ("mount failed\n\r");

	}
	else{
		printf ("mount successfull\n\r");
#endif
	}
	//=====================================================
	//					opening file
	//=====================================================
	res = FR_NO_FILE;
	if(FR_OK != res) // we only try once to open file
	{
		snprintf(filename, sizeof(filename), "data.txt");
		printf("res=%d trying %s\n\r", res, filename);
		res = f_open(&logfile, filename, FA_READ|FA_WRITE);// uses other functions to write, create file
	}
	if (res){								//BAD
		printf("file open failed\n\r");
		LED_ON(2);
		cli();								//clear interrupt enable
	}
	else{									//GOOD
		printf("file opened succesfully\n\r");
//		sei();
	}
	//=====================================================
	//					my stoof
	//=====================================================
	write_result = f_write(&logfile, data_to_write, sizeof(data_to_write),0);
	if(write_result==FR_OK){
		lcd_cls();
		lcd_locate(1,2);
		lcd_str("write cmplt");
		_delay_ms(1000);
		lcd_cls();
	}
	write_result = f_read(&logfile, bytes_read,10,0);
	lcd_locate(0,0);
	lcd_str(bytes_read);
	lcd_locate(0,10);
	lcd_int(write_result);	//read result
	printf("%s\n\r",bytes_read);
	write_result=f_close(&logfile);
	lcd_str(" ");
	lcd_int(write_result);	//close resutl
	//bytes read might need to be 0ed
//	f_open(&logfile, filename, FA_READ);
	f_read(&logfile, bytes_read,10,0);
	lcd_locate(1,0);
	lcd_str(bytes_read);
	printf(bytes_read);
	//=====================================================
	//					MAIN LOOP
	//=====================================================
	while (1){

//		if ((f_write(&logfile, &c, 1, &bytes_written) == FR_OK)
//			&& (1 == bytes_written)){
//			LED_OFF(2);
//		}
//		else
//		{
//			LED_ON(2);
//			cli(); // disable interrupts
//			printf("Oops, only wrote %d bytes\n\r", bytes_written);
//		}

		/* sync the data in case we loose power */

//		if ('\n' == c){
//
//			if (f_sync(&logfile) == FR_OK )
//			{
//				LED_OFF(2);
//			}
//			else
//			{
//				LED_ON(2);
//				cli(); // disable interrupts
//				printf("error syncing to disk.\n\r");
//			}
//		}
	}//end of main loop

	f_mount(0, NULL);
}

//	timer0 setup
/*sei();
	TCCR0A |= 0;
	TCCR0B |= ( 1<< CS00);
	TIMSK0 |= ( 1<<TOIE0);
ISR(TIMER0_OVF_vect){
//	TCNT0 = 240;}
*/
