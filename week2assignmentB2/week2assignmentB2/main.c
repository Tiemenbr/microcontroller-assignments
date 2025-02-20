/*
 * week2assignmentB2.c
 *
 * Created: 14-2-2025 12:19:02
 * Author : tcpma
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

ISR( INT0_vect)
{
	if (PORTA != 0x80) {
		PORTA = PORTA << 1;
	} else {
		PORTA = 0x01;
	}
}

ISR( INT1_vect)
{
	if (PORTA != 0x01)
	{
		PORTA = PORTA >> 1;
	} else {
	PORTA = 0x80;
	}
}

int main(void)
{
	DDRD = 0x00; //set input
	DDRA = 0xFF;
	
	EICRA = 0x0F; // sets interupt 1 and 2 to rising edge
	EIMSK = 0x03; // 0011 sets mask, enables only  
	
	sei();
	
	PORTA = 0x01;
	
    /* Replace with your application code */
    while (1) 
    {
    }
}

