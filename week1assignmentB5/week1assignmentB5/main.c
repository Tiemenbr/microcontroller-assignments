/*
 * Week1AssignmentB2.c
 *
 * Created: 7-2-2025 14:42:39
 * Author : tcpma
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

void wait(int ms) {
	for (int i=0; i<ms; i++)
	{
		_delay_ms(1);
	}
}

int main(void)
{
	DDRD = 0xFF;
	
	int patterns[] = {
		0b10000000,
		0b11000000,
		0b11100000,
		0b01110000,
		0b00111000,
		0b00011100,
		0b00001110,
		0b00000111,
		0b00000011,
		0b00000001
		};
	
    while (1) 
    {
		int i;	
		for (i = 0; i< 10; i++)
		{
			PORTD = patterns[i];
			wait(50);
		}
    }
}


