/*
 * counter_t2.c
 *
 * Created: 21/02/2021 12:45:33
 * Author : tieme
 */ 

#define F_CPU 8e6
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "lcd.h"

int main(void) {
    
	DDRD &= ~(1 << (7));
	DDRA = 0xFF;
	TCCR2 = 0b00000111;
	
	init_4bits_mode();
	wait(10);
	lcd_clear();
	int currentVal;
    while (1) {
		if (currentVal != TCNT2) {
			currentVal = TCNT2;
			PORTA = TCNT2;
			char buffer[4];
			sprintf(buffer, "%d", TCNT2);
			lcd_clear();
			lcd_write_string(buffer);
			wait(10);
		}
    }
}