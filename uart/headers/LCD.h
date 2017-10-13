//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//TODO write  LCDC_FUNC|LCDC_FUNC4B|LCDC_FUNC2L|LCDC_FUNC5x7
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000UL
#ifndef LCD_H_
#define LCD_H_
#define LCD_Y 2
#define LCD_X 16
#define USE_RW 1 // 1 - using; 0 - not
#define LCD_D7PORT C
#define LCD_D7 5
#define LCD_D6PORT C
#define LCD_D6 4
#define LCD_D5PORT C
#define LCD_D5 3
#define LCD_D4PORT C
#define LCD_D4 2
#define LCD_RSPORT B
#define LCD_RS 2	// sending:0 - command; 1 - character to display
#define LCD_RWPORT B
#define LCD_RW 1
#define LCD_EPORT B
#define LCD_E 0		// enable action pin
#if( (LCD_Y == 4) && (LCD_X == 20))
	// TODO
#else
	#define LCD_LINE1 0x00 //adress of 1st char in 1st row
	#define LCD_LINE2 0x40 // 1st char in 2nd row
	#define LCD_LINE3 0x10
	#define LCD_LINE4 0x50 // 1st char in 4th row
#endif

//******************************************
#define PORT(x) SPORT(x)
#define SPORT(x) (PORT##x)

#define PIN(x) SPIN(x)
#define SPIN(x) (PIN##x)

#define DDR(x) SDDR(x)
#define SDDR(x) (DDR##x)

//******************************************
//set or clear given control pin
#define SET_RS PORT(LCD_RSPORT) |= (1<<LCD_RS)
#define CLR_RS PORT(LCD_RSPORT) &= ~(1<<LCD_RS)

#define SET_RW PORT(LCD_RWPORT) |= (1<<LCD_RW)
#define CLR_RW PORT(LCD_RWPORT) &= ~(1<<LCD_RW)

#define SET_E PORT(LCD_EPORT) |= (1<<LCD_E)
#define CLR_E PORT(LCD_EPORT) &= ~(1<<LCD_E)

//******************************************
#define LCD_CLS (1<<0)
// used in LCD initialization on startup
#define LCDC_FUNC (1<<5)		// LCD display settings mode
#define LCDC_FUNC4B ~(1<<4)		// 4bit communication mode
#define LCDC_FUNC2L (1<<3)		// two line display
#define LCDC_FUNC5x7 ~(1<<2)	// 5x7 char
#define LCDC_ONOFF	(1<<3)		//function set mode
#define LCDC_CURSOROFF ~(1<<1)	//turn off cursor
#define LCDC_DISPLAYON (1<<2)	//turn on display
#define LCDC_ENTRY	(1<<2)		//entry mode set
#define LCDC_ENTRYR (1<<1)		//cursor/window goes right


#define USE_LCD_INT 1
#define USE_LCD_HEX 1
/*											*
 * 			FUNCTION DECLARATIONS			*
 * 	 	 	 	 	 	 	 	 	 	 	*/

static inline void lcd_sendHalf(uint8_t);
static inline void data_dir_out(void);
static inline void data_dir_in(void);
#if USE_RW == 1
	static inline uint8_t lcd_readHalf(void);
	uint8_t _lcd_read_byte(void);
	uint8_t check_BF(void);
#endif
void _lcd_write_byte(unsigned char _data);
void lcd_write_cmd(uint8_t);
void lcd_write_data(uint8_t);
void lcd_cls(void);
void lcd_init(void);
void lcd_str(char *);
void lcd_int(int);
void lcd_hex(int);


















#endif /* LCD_H_ */
