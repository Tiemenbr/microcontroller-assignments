/*
 * finalProject.c
 *
 * Created: 28-3-2025 10:09:37
 * Author : Tiemen and Tom
 */ 


#define F_CPU 8e6
#define SCL_CLOCK 100000L // 100kHz I2C speed
#include "HT16K33/display.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>


#include <avr/pgmspace.h>
#include "HT16K33/display.h"
#include "lcd/lcd.h"

#define BIT(x)	(1 << (x))
#define ADCMIN 16
#define ADCMAX 959

int mscount = 0;
int mscount2 = 0;
int ms = 0;
int timerTreshold = 100;
int adcMinPassed = 0;
int adcMaxPassed = 0;

int leds[8] = {0x0F, 0xF0, 0x0F, 0xF0, 0x0F, 0xF0, 0x0F, 0xF0};
//int leds[8] = {0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE}; //set for garenteed win

int rowsDone[8] = {0,0,0,0,0,0,0,0};
int gameDone = 0;

void non (void) {
	//emtpy function to do nothing while ADC is running
}

void (*game)() = &non;


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

//game logic that runs when handle has been pulled.
void rowChecks() {
	int row;
	int completed = 0;
	for (row = 0; row < 8; row++)
	{
		if (rowsDone[row])
		{
			completed++;
			continue;
		}
		
		leds[row] = ((leds[row] << 1) | (leds[row] >> 7)) & 0xFF;
		setrow(row, leds[row]);
		
		//random chance to stop the bit shifting.
		if (rand()%10 == 0)
		{
			rowsDone[row] = 1;
		}
	}
	
	if (completed == 8)
	{
		//game done
		gameDone = 1;
		game = &non;
	}
}


void slotGame (void)
{
	mscount2 = 0;
	timerTreshold = (ms / 5 > 150) ? 150 : ms / 5;
	int row;
	for (row = 0; row < 8; row++)
	{
		rowsDone[row] = 0;
	}
	game = &rowChecks;
}

void adcMin( void )
{
	if (adcMaxPassed)
	{
		ms = mscount;
		adcMaxPassed = 0;
		slotGame();
	}
	adcMinPassed = 1;
	mscount = 0;
}

void adcMax( void )
{
	if (adcMinPassed)
	{
		ms = mscount;
		adcMinPassed = 0;
		slotGame();
	}
	adcMaxPassed = 1;
	mscount = 0;
}

ISR (ADC_vect)
{
	int l = ADCL;
	int h = ADCH;
	
	
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
	mscount2++;
	if (mscount2 > timerTreshold) { 
		game();
		mscount2 = 0;
	}
}

// Main program: ADC at PF1
int main( void )
{
	DDRF = 0x00;				// set PORTF for input (ADC)
	
	timer2init();
	sei();
	adcInit();					// initialize ADC
	init_lcd();					// Initialize lcd
	init_ht16k33();				// Initialize HT16K33 matrix
	
	//set led matrix to show correct lights
	int row;
	for (row = 0; row < 8; row++)
	{
		setrow(row, leds[row]);
	}
	
	//show play text on lcd
	display_text("Play now!");
	
	while (1)
	{
		if (gameDone == 1)
		{
			//check if victory
			if(leds[0] & leds[1] & leds[2] & leds[3] & leds[4] & leds[5] & leds[6] & leds[7])
			{
				//won
				display_text("You won!");
			} else {
				//lost
				display_text("oops try again");
			}
			gameDone = 0;
		}
	}
}
