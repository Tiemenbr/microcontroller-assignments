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
	DDRC = 0;
	
    while (1) 
    {
		int i = 1;
		for(; PINC & 0x01; i = i ^ 1)
		{
			PORTD = i;
			wait(500);
		}
    }
}


