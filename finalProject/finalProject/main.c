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

#include <avr/pgmspace.h>
#include "HT16K33/display.h"

#define BIT(x)	(1 << (x))
#define ADCMIN 16
#define ADCMAX 959

int mscount = 0;
int adcMinPassed = 0;
int adcMaxPassed = 0;

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

void timer2init()
{
	OCR2 = 125;
	TCCR2 = 0b00001011;
	TIMSK |= (1<<7);
}


void adcMin( void )
{
	if (adcMaxPassed)
	{
		PORTC = 1;
		PORTD = mscount;
		adcMaxPassed = 0;
	}
	adcMinPassed = 1;
	mscount = 0;
}

void adcMax( void )
{
	if (adcMinPassed)
	{
		PORTC = 3;
		PORTD = mscount;
		adcMinPassed = 0;
	}
	adcMaxPassed = 1;
	mscount = 0;
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
}

ISR(TIMER2_COMP_vect)
{
	mscount++;
}

// Main program: ADC at PF1
int main( void )
{
	/*
	DDRF = 0x00;				// set PORTF for input (ADC)
	DDRA = 0xFF;				// set PORTA for output 
	DDRB = 0xFF;				// set PORTB for output
	DDRC = 0xFF;				// set PORTC for output
	DDRD = 0xFF;				// set PORTD for output
	
	timer2init();
	sei();
	adcInit();					// initialize ADC
	*/
	
	displayInit();
	wait(500);

	while (1)
	{
		displayClr();
		displaySetPixel(1,1);
		display();
	}
}
