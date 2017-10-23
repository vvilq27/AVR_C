#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include "headers/LCD.h"

// RS = 1 -> write data RS = 0 -> write command
// RW = 1 -> read mode RW = 0 -> write mode

//static - function not visible outside this file
//inline - this code should go straight to flash memory everywhere this function is invoked(
static inline void lcd_sendHalf(uint8_t data){
	if (data & (1<<0)) PORT(LCD_D4PORT) |= (1<<LCD_D4);
	else PORT(LCD_D4PORT) &= ~(1<<LCD_D4);
	if (data & (1<<1)) PORT(LCD_D5PORT) |= (1<<LCD_D5);
	else PORT(LCD_D5PORT) &= ~(1<<LCD_D5);
	if (data & (1<<2)) PORT(LCD_D6PORT) |= (1<<LCD_D6);
	else PORT(LCD_D6PORT) &= ~(1<<LCD_D6);
	if (data & (1<<3)) PORT(LCD_D7PORT) |= (1<<LCD_D7);
	else PORT(LCD_D7PORT) &= ~(1<<LCD_D7);
}
// reading from LCD
#if USE_RW == 1
	static inline uint8_t lcd_readHalf(void){
		uint8_t read_data = 0;
		if(PIN(LCD_D4PORT)&(1<<LCD_D4)) read_data |= (1<<0);
		if(PIN(LCD_D5PORT)&(1<<LCD_D5)) read_data |= (1<<1);
		if(PIN(LCD_D6PORT)&(1<<LCD_D6)) read_data |= (1<<2);
		if(PIN(LCD_D7PORT)&(1<<LCD_D7)) read_data |= (1<<3);
		return read_data;
	}
	uint8_t _lcd_read_byte(void){
		uint8_t read_data = 0;
		data_dir_in();
		SET_RW;
		SET_E;
		read_data |= (lcd_readHalf() << 4);//read top half form LCD
		CLR_E;
		//TODO: make here somehow pause and check if LCD willsend second half without seting enable bit
		SET_E;
		read_data |= lcd_readHalf();//read bottom half from LCD
		CLR_E;
		return read_data;
	}
	uint8_t check_BF(void){	//should return boolean value, makes no sense this way
		CLR_RS;
		return _lcd_read_byte();
	}
#endif

static inline void data_dir_out(void){
	DDR(LCD_D4PORT) |= (1<<LCD_D4);
	DDR(LCD_D5PORT) |= (1<<LCD_D5);
	DDR(LCD_D6PORT) |= (1<<LCD_D6);
	DDR(LCD_D7PORT) |= (1<<LCD_D7);
}
static inline void data_dir_in(void){
	DDR(LCD_D4PORT) &= ~(1<<LCD_D4);
	DDR(LCD_D5PORT) &= ~(1<<LCD_D5);
	DDR(LCD_D6PORT) &= ~(1<<LCD_D6);
	DDR(LCD_D7PORT) &= ~(1<<LCD_D7);
}
void _lcd_write_byte(unsigned char _data){
	//set D4-D7 pins as output
	data_dir_out();

	//sending data => got to clear RW pin
	#if USE_RW == 1
		CLR_RW;
	#endif

	SET_E;
	lcd_sendHalf(_data>>4);// send top half of data byte
	CLR_E;


	SET_E;
	lcd_sendHalf(_data); // send bottom half of data byte
	CLR_E;

	#if USE_RW == 1
		//TRICK: 7 bits out of 8 are masked by AND function so basically result (true/false) depends only on 8th bit value
		while( check_BF() & (1<<7)); //waiting for
		//TODO timer counting BF period
	#else
		_delay_us(120);
	#endif
}
void lcd_write_cmd(uint8_t cmd){
	CLR_RS;
	_lcd_write_byte(cmd);
}
void lcd_write_data(uint8_t data){
	SET_RS;
	_lcd_write_byte(data);
}
void lcd_cls(void){
	lcd_write_cmd(LCD_CLS);

	#if USE_RW == 0
		_delay_ms(4.9);
	#endif
}
void lcd_init(void){
	data_dir_out();
	DDR(LCD_RSPORT) |= (1<<LCD_RS);
	DDR(LCD_EPORT) |= (1<<LCD_E);
	#if USE_RW == 1	//RW as output pin
		DDR(LCD_RWPORT) |= (1<<LCD_RW);
	#endif
	PORT(LCD_RSPORT) |= (1<<LCD_RS);
	PORT(LCD_EPORT) |= (1<<LCD_E);
	#if USE_RW == 1 // RW in read mode
		PORT(LCD_RWPORT) |= (1<<LCD_RW);
	#endif
	_delay_ms(15);
	PORT(LCD_EPORT) &= ~(1<<LCD_E); // CLR_E
	PORT(LCD_RSPORT) &= ~(1<<LCD_RS);	// CLR_RS
	//setting LCD to operate on 4 data bits
	#if USE_RW == 1
		PORT(LCD_RWPORT) &= ~(1<<LCD_RW);
		SET_E;
		lcd_sendHalf(0x03); //8 pin mode, DL =1
		CLR_E;
		_delay_ms(4.1);
		SET_E;
		lcd_sendHalf(0x03); //8 pin mode, DL =1
		CLR_E;
		_delay_us(100);
		SET_E;
		lcd_sendHalf(0x03); //8 pin mode, DL =1
		CLR_E;
		_delay_us(100);
		SET_E;
		lcd_sendHalf(0x02);// 4 bit mode, DL = 0
		CLR_E;
		_delay_us(100);
		// now we can check busy flag
		lcd_write_cmd(LCDC_FUNC|LCDC_FUNC4B|LCDC_FUNC2L|LCDC_FUNC5x7);//settings: 4 bit data, 2 rows, char 5x7
		lcd_write_cmd(LCDC_ONOFF|LCDC_CURSOROFF);//turns off cursor
		lcd_write_cmd(LCDC_ONOFF|LCDC_DISPLAYON);//turns on LCD
		lcd_write_cmd(LCDC_ENTRY|LCDC_ENTRYR);//shifts cursor right without moving screen content
		lcd_cls();//clear LCD
	#endif
}
#if USE_LCD_CURSOR_HOME == 1
	void lcd_home(void){
		lcd_write_cmd(LCDC_CLS|LCDC_HOME);

		#if USE_RW == 0
				_delay_ms(4.9);
		#endif
	}
#endif
#if USE_LCD_CURSOR_ON == 1
	void lcd_cursor_on(void){
		lcd_write_cmd(LCDC_ONOFF|LCDC_DISPLAYON|LCDC_LCDC_CURSORON);
	}
	void lcd_cursor_off(void){
		lcd_write_cmd(LCDC_ONOFF|LCDC_DISPLAYON);
	}
#endif
#if USE_LCD_CURSOR_BLINK == 1
	//TODO p 228
#endif
//char sources functions:
//display RAM-defined strings
//remember that after string is finished in table it is followed by 0 - meaning end of string,easy to check with while
//void lcd_str(char * str){
//	while(*str) lcd_write_data(*str++);
//}

//void lcd_char(char c){
//	if(c == 0xa || c==0xd){
//
//	}else{
//		lcd_locate(1,0);
//		while(UART_RxBuff[UART_RxTail++] != 0){
//		lcd_write_data(c);
//		}
//	}
//}

void lcd_str(char * str){
	register char znak;
	while( (znak =* (str++)) )
		lcd_write_data( (znak>= 0x80 && znak <= 0x87) ? (znak & 0x07): znak);
}


// _P indicates that it has something to do with FLASH mem address
#if USE_LCD_STR_P == 1
	void lcd_str_P(char * str){
		register char character;
		while( ( character = pgm_read_byte(str++)) )
			lcd_write_data( ( (character >= 0x80) && (character<= 0x87) ) ? (character & 0x07) : character);
	}
#endif
// _E EEPROM mem address
#if USE_LCD_STR_E == 1
	void lcd_str_E(char * str){
		register char character;
		while(1){
			character = eeprom_read_byte( (uint8_t *) (str++) ); // cast to a 8bit pointer
			if(!character || character == 0xFF)
				break; // we gonna treat 0xff as the beginning of EEPROM
			else
				lcd_write_data( ( (character=>0x80) && (character<=0x87) ) ? (character & 0x07) : character);
		}
	}
#endif

//PSTR(string)
//compiler defines in FLASH char table and result of this func is its address
//lcd_str_P( PSTR("message in FLASH mem") );
//WARNING: display func has to use FLASH addresses, lcd_str wont work here

#if USE_LCD_INT == 1
	void lcd_int(int val){
		char buffer[17];
		lcd_str( itoa(val, buffer, 10) );
	}
#endif
#if USE_LCD_HEX == 1
	void lcd_hex(int val){
		char buffer[17];
		lcd_str( itoa(val, buffer, 16) );
	}
#endif

//itoa(value_to_change, address_to_store_changed_data, changed_data_format);
//operating range -/+ 32k
//formats:
//10 - decimal
//16 - hex
#if USE_LOCATE == 1
	void lcd_locate(uint8_t y, uint8_t x){
		switch(y){
			case 0: y = LCD_LINE1; break;
			#if (LCD_Y>1)
				case 1: y = LCD_LINE2; break;
			#endif
			#if (LCD_Y > 2)
				case 2: y = LCD_LINE3; break;
			#endif
			#if (LCD_Y > 3)
				case 3: y = LCD_LINE4; break
			#endif
		}
		lcd_write_cmd( (0x80+y+x) );
	}
#endif


// INFO: new character function ommited

































