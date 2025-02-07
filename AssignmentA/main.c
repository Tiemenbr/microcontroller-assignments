/*
 * AssignmentA.c
 *
 * Created: 7-2-2025 14:10:30
 * Author : Tiemen and Tom
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
	DDRA = 0xFF;
	
	while (1)
	{
		PORTA = 0xF0;
		wait(1000);
		PORTA = 0x0F;
		wait(1000);
	}
}

