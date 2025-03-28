/*
 * finalProject.c
 *
 * Created: 28-3-2025 10:09:37
 * Author : Tiemen and Tom
 */ 


#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BIT(x)	(1 << (x))
#define ADCMIN 16
#define ADCMAX 959

// wait(): busy waiting for 'ms' millisecond
// Used library: util/delay.h
void wait( int ms )
{
	for (int tms=0; tms<ms; tms++)
	{
		_delay_ms( 1 );			// library function (max 30 ms at 8MHz)
	}
}


// Initialize ADC: 10-bits (left justified), free running
void adcInit( void )
{
	ADMUX = 0b01100001;			// AREF=VCC, result left adjusted, channel1 at pin PF1
	ADCSRA = 0b11101110;		// ADC-enable, interrupt, start, no free running, division by 64
}

void adcMin( void )
{
	PORTC = 1;
}

void adcMax( void )
{
	PORTC = 3;
}


ISR (ADC_vect)
{
	int l = ADCL;
	int h = ADCH;

	PORTB = l;			// Show MSB/LSB (bit 10:0) of ADC
	PORTA = h;
		
		
	int adcOut = (h << 2) | l;
	if (adcOut < ADCMIN)
	{
		adcMin();
	}
	else if (adcOut > ADCMAX) 
	{
		adcMax();
	}
	else 
	{
		PORTC = 0;
	}
}

// Main program: ADC at PF1
int main( void )
{
	DDRF = 0x00;				// set PORTF for input (ADC)
	DDRA = 0xFF;				// set PORTA for output 
	DDRB = 0xFF;				// set PORTB for output
	DDRC = 0xFF;				// set PORTC for output
	sei();
	adcInit();					// initialize ADC
	
	while (1)
	{
		
	}
}
