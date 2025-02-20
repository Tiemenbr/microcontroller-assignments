/*
 * week2assignmentB4.c
 *
 * Created: 14-2-2025 13:30:19
 * Author : tieme
 */ 

#define F_CPU 8e6

#include <avr/io.h>
#include <util/delay.h>

typedef struct {
	unsigned char data;
	unsigned int delay ;
} PATTERN_STRUCT;

PATTERN_STRUCT pattern[] = {
	//  Dgfedcba
	{ 0b00000011, 50 },//ab
	{ 0b01000010, 50 },//bg
	{ 0b01010000, 50 },//ge
	{ 0b00011000, 50 },//ed
	{ 0b00001100, 50 },//dc
	{ 0b01000100, 50 },//cg
	{ 0b01100000, 50 },//gf
	{ 0b00100001, 50 },//fa
	{ 0x00, 0x00 } //terminator
};

void wait( int ms ) {
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );
	}
}

int main( void )
{
	DDRB = 0xFF; //enable write
	
	while (1) {
		// Set index to begin of pattern array
		int index = 0;
		// as long as delay has meaningful content
		while( pattern[index].delay != 0 ) {
			// Write data to PORTB
			PORTB = pattern[index].data;
			// wait
			wait(pattern[index].delay);
			// increment for next round
			index++;
		}
	}

	return 1;
}