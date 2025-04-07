/*
 * lcd.h
 *
 * Created: 14-2-2025 13:52:50
 *  Author: tieme
 */ 


#ifndef LCD_H_
#define LCD_H_

void init();
void display_text(char *str);
void set_cursor(int position);

#endif /* LCD_H_ */