/*
 * lcd.h
 *
 * Created: 21-2-2025 11:38:04
 *  Author: tcpma
 */ 

#ifndef LCD_H
#define LCD_H

void lcd_strobe_lcd_e(void);
void init_4bits_mode(void);
void lcd_write_string(char *str);
void lcd_write_data(unsigned char byte);
void lcd_write_cmd(unsigned char byte);
void lcd_clear(void);
void wait( int ms );

#endif