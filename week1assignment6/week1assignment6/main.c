/*
 * Week1AssignmentB2.c
 *
 * Created: 7-2-2025 14:42:39
 * Author : tcpma
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

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
	
	bool state = false;
	PORTD = 1 << 7;

	int counter = 0; 	

    while (1) 
    {
		
		if (PINC & 1) 
		{
			state = !state;
		}
		
		if (state)
		{
			if (counter > 1000)
			{
				PORTD = PORTD ^ (1 << 7);
				counter = 0;
			}
		}
		else
		{
			if (counter > 250)
			{
				PORTD = PORTD ^ (1 << 7);
				counter = 0;
			}
		}
		counter += 20;
		_delay_ms(20);
    }
}




