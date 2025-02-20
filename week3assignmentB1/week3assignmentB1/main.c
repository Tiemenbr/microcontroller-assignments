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

void wait( int ms ) {
	for (int tms=0; tms<ms; tms++) {
		_delay_ms( 1 );	
	}
}


int main(void) {
    
	DDRD &= ~(1 << (7));
	DDRA = 0xFF;
	TCCR2 = 0b00000111;

    while (1) {
		PORTA = TCNT2;
		wait(10);
    }
}