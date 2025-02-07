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
	
	int index = 1;
	
    while (1) 
    {
		PORTD = index;
		wait(50);
		index = index << 1;
		if (index == 1 << 8)
		{
			index = 1;
		}
    }
}


