/*
 * week2assignmentB3.c
 *
 * Created: 14-2-2025 12:48:19
 * Author : tcpma
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

void wait (int ms)
{
	for (int i=0; i<ms; i++)
	{
		_delay_ms(1);
	}
}

void display(int digit)
{
	const unsigned char HexNumbers [16] = {
		0b00111111, // 0
		0b00000110, // 1
		0b01011011, // 2
		0b01001111, // 3
		0b01100110, // 4
		0b01101101, // 5
		0b01111101, // 6
		0b00000111, // 7
		0b01111111, // 8
		0b01101111, // 9
		0b01110111, // a
		0b01111100, // b
		0b00111001, // c
		0b01011110, // d
		0b01111001, // e
		0b01110001 // f
	};
	
	if (digit > 15 || digit < 0)
	{
		PORTB = HexNumbers[14];
		return;
	}
	
	PORTB = HexNumbers[digit];
}

int main(void)
{
	DDRA = 0xFF;
	DDRB = 0xFF; //enable write
	DDRD = 0x00; //enable read
	
	int index = 1;
	
    while (1) 
    {
		if (PIND & 0x01)
		{
			index--;
		} 
		else if (PIND & 0x02)
		{
			index++;
		}
		
		display(index);
		PORTA = index; //debugging write index
		
		wait(100); //to prevent dubble clicking
    }
}

