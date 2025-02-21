/*
 * counter_t2.c
 *
 * Created: 21/02/2021 12:45:33
 * Author : tieme
 */ 

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


// Define for UNI_DS_6 Board
// Command this line if you are using a BIGAVR
#define UNI_DS_6


#ifdef UNI_DS_6
#define LCD_E 	6  // RA6 UNI-6
#define LCD_RS	4  // RA4 UNI-6
#define PORT_COMMAND PORTA
#define PORT_DATA PORTC
#else
#define LCD_E 	3
#define LCD_RS	2
#define PORT_COMMAND PORTC
#define PORT_DATA PORTC
#endif

void wait( int ms ) {
	for (int tms=0; tms<ms; tms++) {
		_delay_ms( 1 );	
	}
}


/******************************************************************
short:			Strobe LCD module E pin --__
inputs:
outputs:
notes:			According datasheet HD44780
Version :    	DMK, Initial code
*******************************************************************/
void lcd_strobe_lcd_e(void) {
	PORT_COMMAND |= (1<<LCD_E);	// E high
	_delay_ms(1);			// nodig
	PORT_COMMAND &= ~(1<<LCD_E);  	// E low
	_delay_ms(1);			// nodig?
}


/******************************************************************
short:			Init LCD module in 4 bits mode.
inputs:
outputs:
notes:			According datasheet HD44780 table 12
Version :    	DMK, Initial code
*******************************************************************/
void init_4bits_mode(void) {
	// PORTC output mode and all low (also E and RS pin)
	DDRC = 0xFF;
	
	#ifdef UNI_DS_6
	DDRA = 0xFF;
	PORTA = 0x00;
	#endif
	
	PORT_DATA = 0x00;
	//PORTA = 0xFF;

	// Step 2 (table 12)
	PORT_DATA = 0x20;	// function set
	lcd_strobe_lcd_e();

	// Step 3 (table 12)
	PORT_DATA = 0x20;   // function set
	lcd_strobe_lcd_e();
	PORT_DATA = 0x80;
	lcd_strobe_lcd_e();

	// Step 4 (table 12)
	PORT_DATA = 0x00;   // Display on/off control
	lcd_strobe_lcd_e();
	PORT_DATA = 0xF0;
	lcd_strobe_lcd_e();

	// Step 4 (table 12)
	PORT_DATA = 0x00;   // Entry mode set
	lcd_strobe_lcd_e();
	PORT_DATA = 0x60;
	lcd_strobe_lcd_e();
}

/******************************************************************
short:			Writes 8 bits DATA to lcd
inputs:			byte - written to LCD
outputs:
notes:			According datasheet HD44780 table 12
Version :    	DMK, Initial code
*******************************************************************/
void lcd_write_data(unsigned char byte) {
	// First nibble.
	PORT_DATA = byte;
	PORT_COMMAND |= (1<<LCD_RS);
	lcd_strobe_lcd_e();

	// Second nibble
	PORT_DATA = (byte<<4);
	PORT_COMMAND |= (1<<LCD_RS);
	lcd_strobe_lcd_e();
}

/******************************************************************
short:			Writes string to LCD at cursor position
inputs:
outputs:
notes:			According datasheet HD44780 table 12
Version :    	DMK, Initial code
*******************************************************************/
void lcd_write_string(char *str) {
	// Het kan met een while:

	// while(*str) {
	// 	lcd_write_data(*str++);
	// }

	// of met een for:
	for(;*str; str++){
		lcd_write_data(*str);
	}
}

/******************************************************************
short:			Writes 8 bits COMMAND to lcd
inputs:			byte - written to LCD
outputs:
notes:			According datasheet HD44780 table 12
Version :    	DMK, Initial code
*******************************************************************/
void lcd_write_command(unsigned char byte) {
	// First nibble.
	PORT_DATA = byte;
	PORT_COMMAND &= ~(1<<LCD_RS);
	lcd_strobe_lcd_e();

	// Second nibble
	PORT_DATA = (byte<<4);
	PORT_COMMAND &= ~(1<<LCD_RS);
	lcd_strobe_lcd_e();
}


void lcd_clear() {
	lcd_write_command (0x01);						//Leeg display
	_delay_ms(2);
	lcd_write_command (0x80);						//Cursor terug naar start
}

int main(void) {
    
	DDRD &= ~(1 << (7));
	DDRA = 0xFF;
	TCCR2 = 0b00000111;
	
	init_4bits_mode();
	wait(10);
	lcd_clear();

    while (1) {
		PORTA = TCNT2;
		wait(10);
    }
}