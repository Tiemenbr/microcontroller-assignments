/*
 * test.c
 *
 * Created: 20-2-2025 10:48:33
 *  Author: tieme
 */ 

#define F_CPU 8e6

#include <avr/io.h>
#include <util/delay.h>

void tester_func() 
{
	DDRD = 0xFF;
	PORTD = 0xFF;
	for (;;) 
	{
		PORTD ^= 0xFF;
		_delay_ms(250);
	}
}