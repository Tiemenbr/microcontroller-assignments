/*
 * week3assignmentB2.c
 *
 * Created: 20-2-2025 12:10:49
 * Author : tcpma
 */ 
#define F_CPU 8e6

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>

int mscount = 0;
bool highlowbool = false;

void timer2init()
{
	OCR2 = 125;
	TCCR2 = 0b00001011;
	TIMSK |= (1<<7);
}

ISR(TIMER2_COMP_vect)
{
	mscount++;
	if (highlowbool)
	{
		if (mscount >= 15)
		{
			PORTD &= ~(1 << 7);
			highlowbool = false;
			mscount = 0;
		}
	} else {
		if (mscount >= 25)
		{
			PORTD |= 1 << 7;
			highlowbool = true;
			mscount = 0;
		}
	}
}

int main(void)
{
	DDRD = 1 << 7;
	
	timer2init();
	sei();
	
	
    while (1) 
    {
		_delay_ms(1);
    }
}

