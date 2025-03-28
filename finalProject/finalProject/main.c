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

#define DDR_SPI		DDRB					// spi Data direction register
#define PORT_SPI	PORTB					// spi Output register
#define SPI_SCK		1						// PB1: spi Pin System Clock
#define SPI_MOSI	2						// PB2: spi Pin MOSI
#define SPI_MISO	3						// PB3: spi Pin MISO
#define SPI_SS		0						// PB0: spi Pin Slave Select

void spi_masterInit(void)
{
	DDR_SPI = 0xff;							// All pins output: MOSI, SCK, SS, SS_display as output
	DDR_SPI &= ~BIT(SPI_MISO);				// 	except: MISO input
	PORT_SPI |= BIT(SPI_SS);				// SS_ADC == 1: deselect slave
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1);	// or: SPCR = 0b11010010;
	// Enable spi, MasterMode, Clock rate fck/64, bitrate=125kHz
	// Mode = 0: CPOL=0, CPPH=0;
}


// Write a byte from master to slave
void spi_write( unsigned char data )
{
	SPDR = data;							// Load byte to Data register --> starts transmission
	while( !(SPSR & BIT(SPIF)) ); 			// Wait for transmission complete
}

// Write a byte from master to slave and read a byte from slave
// nice to have; not used here
char z( unsigned char data )
{
	SPDR = data;							// Load byte to Data register --> starts transmission
	while( !(SPSR & BIT(SPIF)) ); 			// Wait for transmission complete
	data = SPDR;							// New received data (eventually, MISO) in SPDR
	return data;							// Return received byte
}

// Select device on pinnumer PORTB
void spi_slaveSelect(unsigned char chipNumber)
{
	PORTB &= ~BIT(chipNumber);
}

// Deselect device on pinnumer PORTB
void spi_slaveDeSelect(unsigned char chipNumber)
{

	PORTB |= BIT(chipNumber);
}


// Initialize the driver chip (type MAX 7219)
void displayDriverInit()
{
	spi_slaveSelect(0);				// Select display chip (MAX7219)
	spi_write(0x09);      			// Register 09: Decode Mode
	spi_write(0xFF);				// 	-> 1's = BCD mode for all digits
	spi_slaveDeSelect(0);			// Deselect display chip

	spi_slaveSelect(0);				// Select dispaly chip
	spi_write(0x0A);      			// Register 0A: Intensity
	spi_write(0x0F);    			//  -> Level 15 (in range [1..F])
	spi_slaveDeSelect(0);			// Deselect display chip

	spi_slaveSelect(0);				// Select display chip
	spi_write(0x0B);  				// Register 0B: Scan-limit
	spi_write(0x03);   				// 	-> 3 = Display digits 0..3
	spi_slaveDeSelect(0);			// Deselect display chip

	spi_slaveSelect(0);				// Select display chip
	spi_write(0x0C); 				// Register 0B: Shutdown register
	spi_write(0x01); 				// 	-> 1 = Normal operation
	spi_slaveDeSelect(0);			// Deselect display chip
}

// Set display on ('normal operation')
void displayOn()
{
	spi_slaveSelect(0);				// Select display chip
	spi_write(0x0C); 				// Register 0B: Shutdown register
	spi_write(0x01); 				// 	-> 1 = Normal operation
	spi_slaveDeSelect(0);			// Deselect display chip
}

// Set display off ('shut down')
void displayOff()
{
	spi_slaveSelect(0);				// Select display chip
	spi_write(0x0C); 				// Register 0B: Shutdown register
	spi_write(0x00); 				// 	-> 1 = Normal operation
	spi_slaveDeSelect(0);			// Deselect display chip
}

// Write a word = address byte + data byte from master to slave
void spi_writeWord ( unsigned char adress, unsigned char data )
{
	spi_slaveSelect(0);         // Select display chip
	spi_write(adress);         		// 	digit adress: (digit place)
	spi_write(data);  		// 	digit value: i (= digit place)
	spi_slaveDeSelect(0); 		// Deselect display chip
}

int power(int base, int exponent)
{
	if (exponent == 0)
	{
		return 1;
	}
	int ret = base;
	for(int i = 1; i<exponent; i++)
	{
		ret = ret*base;
	}
	return ret;
}

// toont de waarde van value op het 4-digit display
void writeLedDisplay( int value )
{
	if (value>=10000 || value <=-1000)
	{
		return;
	}
	if (value >= 0)
	{
		for(int i=1; i<=4; i++)
		{
			spi_writeWord(i,(value%power(10,i))/power(10,i-1));
		}
	}
	else
	{
		value = value*-1;
		for(int i=1; i<=3; i++)
		{
			spi_writeWord(i,(value%power(10,i))/power(10,i-1));
		}
		spi_writeWord(4,10);
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
		PORTD = mscount;
		writeLedDisplay(mscount);
		adcMaxPassed = 0;
	}
	adcMinPassed = 1;
	mscount = 0;
}

void adcMax( void )
{
	if (adcMinPassed)
	{
		PORTD = mscount;
		writeLedDisplay(mscount);
		adcMinPassed = 0;
	}
	adcMaxPassed = 1;
	mscount = 0;
}

ISR (ADC_vect)
{
	int l = ADCL;
	int h = ADCH;

	//PORTB = l;			// Show MSB/LSB (bit 10:0) of ADC
	//PORTA = h;
	
	
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
	DDRF = 0x00;				// set PORTF for input (ADC)
	DDRA = 0xFF;				// set PORTA for output 
	//DDRB = 0xFF;				// set PORTB for output
	DDRC = 0xFF;				// set PORTC for output
	DDRD = 0xFF;				// set PORTD for output
	
	DDRB = 0x01;
	spi_masterInit();              	// Initialize spi module
	displayDriverInit();            // Initialize display chip
	
	// clear display (all zero's)
	for (char i =1; i<=4; i++)
	{
		spi_slaveSelect(0); 		// Select display chip
		spi_write(i);  				// 	digit adress: (digit place)
		spi_write(0);				// 	digit value: 0
		spi_slaveDeSelect(0);		// Deselect display chip
	}
	
	timer2init();
	sei();
	adcInit();					// initialize ADC
	
	while (1)
	{
		
	}
}
