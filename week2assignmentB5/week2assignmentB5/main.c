/*
 * week2assignmentB5.c
 *
 * Created: 14-2-2025 13:50:56
 * Author : tieme
 */ 

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

void wait( int ms ) {
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );
	}
}


int main(void)
{
    // Init I/O
    DDRD = 0xFF;			// PORTD(7) output, PORTD(6:0) input

    // Init LCD
    init();

    // Write sample string
    display_text("Yedi you are");
	set_cursor(16);
	display_text("Bottom text");

    // Loop forever
    while (1) {
	    PORTD ^= (1<<7);	// Toggle PORTD.7
	    _delay_ms( 250 );
    }

    return 1;
}

