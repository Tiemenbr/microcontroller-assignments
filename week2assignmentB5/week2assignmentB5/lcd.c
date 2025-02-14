/*
 * lcd.c
 *
 * Created: 14-2-2025 13:53:06
 *  Author: tieme
 */ 

#include "init"

typedef unsigned char byte;


void lcd_strobe_lcd_e(void)
{
	PORTA |= (1<<6);
	_delay_ms(1);
	PORTA &= ~(1<<6);
	_delay_ms(1);
}

void lcd_cmd_high_nibble(byte cmd)
{
	PORTC = (cmd & 0xF0); // High nibble to PC4-7
	PORTA &= ~(1<<4); // Force RS=PA4 low
}

void lcd_cmd_low_nibble(byte cmd)
{
	PORTC = (cmd & 0x0F) << 4; // Low nibble to PC4-7
	PORTA &= ~(1<<4); // Force RS=PA4 low
}

void lcd_data_high_nibble(byte data)
{
	PORTC = (data & 0xF0); // High nibble to PC4-7
	PORTA |= (1<<4); // Force RS=PA4 high
}

void lcd_data_low_nibble(byte data)
{
	PORTC = (data & 0x0F) << 4; // Low nibble to PC4-7
	PORTA |= (1<<4); // Force RS=PA4 high
}

void lcd_command(byte cmd)
{
	lcd_cmd_high_nibble(cmd);
	lcd_strobe_lcd_e();
	lcd_cmd_low_nibble(cmd);
	lcd_strobe_lcd_e();
}

void lcd_data(byte data)
{
	lcd_data_high_nibble(data);
	lcd_strobe_lcd_e();
	lcd_data_low_nibble(data);
	lcd_strobe_lcd_e();
}


void init(void)
{
	// return home
	lcd_command( 0x02 );
	// mode: 4 bits interface data, 2 lines, 5x8 dots
	lcd_command( 0x28 );
	// display: on, cursor off, blinking off
	lcd_command( 0x0C );
	// entry mode: cursor to right, no shift
	lcd_command( 0x06 );
	// RAM address: 0, first position, line 1
	lcd_command( 0x80 );
}