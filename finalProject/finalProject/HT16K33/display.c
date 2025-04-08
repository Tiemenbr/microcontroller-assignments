/*
 * display.c
 *
 * Created: 8-4-2025 11:22:40
 *  Author: tieme
 */ 

#include <avr/io.h>
#include <util/delay.h>


// I2C setup functions (already defined in previous steps)
void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
uint8_t i2c_write(uint8_t data);


// HT16K33 I2C address
#define HT16K33_ADDR 0x74  // 7-bit address for your HT16K33 matrix
uint8_t display_buffer[16] = {0};  // Mirrors HT16K33's display RAM (16 bytes)


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

// Initialize the HT16K33 (turn on oscillator, enable display)
void ht16k33_init(void) {
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
}

// Function to clear the matrix (turn off all pixels)
void clear_matrix(void) {
	for (uint8_t col = 0; col < 8; col++) {
		uint8_t addr = col * 2;
		display_buffer[addr] = 0x00;

		i2c_start();
		i2c_write((HT16K33_ADDR << 1) | 0);
		i2c_write(addr);
		i2c_write(0x00);
		i2c_stop();
	}
}

void set_pixel(uint8_t row, uint8_t col, uint8_t on) {
	if (row > 7 || col > 7) return; // Bounds check

	uint8_t addr = col * 2;               // Even addresses: 0x00, 0x02, ..., 0x0E
	uint8_t bit = (1 << row);             // Each bit is one row

	if (on)
	display_buffer[addr] |= bit;
	else
	display_buffer[addr] &= ~bit;

	// Send to display
	i2c_start();
	i2c_write((HT16K33_ADDR << 1) | 0);
	i2c_write(addr);
	i2c_write(display_buffer[addr]);
	i2c_stop();
}