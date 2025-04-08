/*
 * display.c
 *
 * Created: 8-4-2025 11:22:40
 *  Author: tieme
 */ 
#define F_CPU 8e6
#define SCL_CLOCK 100000L // 100kHz I2C speed
#include <avr/io.h>
#include <util/delay.h>
#include "display.h"

// HT16K33 I2C address
#define HT16K33_ADDR 0x74  // 7-bit address for your HT16K33 matrix

void i2c_init(void) {
	// Set SCL frequency
	TWSR = 0x00; // Prescaler = 1
	TWBR = ((F_CPU / SCL_CLOCK) - 16) / 2;

	// Enable TWI
	TWCR = (1 << TWEN);
}

void i2c_start(void) {
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); // Send START
	while (!(TWCR & (1 << TWINT))); // Wait for TWINT flag
}

void i2c_stop(void) {
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); // Send STOP
	while (TWCR & (1 << TWSTO)); // Wait for STOP to complete
}

uint8_t i2c_write(uint8_t data) {
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN); // Start transmission
	while (!(TWCR & (1 << TWINT))); // Wait for complete
	return (TWSR & 0xF8); // Return status code
}

// HT16K33 I2C address
#define HT16K33_ADDR 0x74  // 7-bit address for your HT16K33 matrix


void init_ht16k33(void) {
	i2c_init();
	_delay_ms(10);  // Small delay to ensure i2c is enabled
	
	i2c_start();
	i2c_write((HT16K33_ADDR << 1) | 0);  // Write mode
	i2c_write(0x21); // Turn on oscillator
	i2c_stop();
	_delay_ms(10);  // Small delay to ensure oscillator is enabled
	
	i2c_start();
	i2c_write((HT16K33_ADDR << 1) | 0);  // Write mode
	i2c_write(0xA0); // HT16K33 pins all output (default)
	i2c_stop();
	_delay_ms(10);

	i2c_start();
	i2c_write((HT16K33_ADDR << 1) | 0);
	i2c_write(0x81); // Display on, no blink
	i2c_stop();
	_delay_ms(10);  // Small delay for display to turn on

	i2c_start();
	i2c_write((HT16K33_ADDR << 1) | 0);
	i2c_write(0xEF); // Brightness: max
	i2c_stop();
	_delay_ms(10);  // Small delay for brightness adjustment
	
	clear_matrix();
}

// Function to clear the matrix (turn off all pixels)
void clear_matrix(void) {
	for (int col = 0; col < 8; col++) {
		int addr = col * 2;

		i2c_start();
		i2c_write((HT16K33_ADDR << 1) | 0);
		i2c_write(addr);
		i2c_write(0x00);
		i2c_stop();
	}
}

void setrow(int row, int vallue)
{
	if (row < 0 || row > 8) return; //out of bounds
	int addr = row * 2; 
	int led_vallues = (vallue >> 1) | ((vallue & 0x01) << 7);
	
	i2c_start();
	i2c_write((HT16K33_ADDR << 1) | 0);
	i2c_write(addr);
	i2c_write(led_vallues);
	i2c_stop();
}


void set_pixel(uint8_t row, uint8_t col, uint8_t on) {
	if (row > 7 || col > 7) return; // Bounds check

	uint8_t addr = col * 2;               // Even addresses: 0x00, 0x02, ..., 0x0E
	uint8_t bit = (1 << row);             // Each bit is one row

	// Send to display
	i2c_start();
	i2c_write((HT16K33_ADDR << 1) | 0);
	i2c_write(addr);
	i2c_write(bit);
	i2c_stop();
}