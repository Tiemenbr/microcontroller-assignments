/*
 * display.c
 *
 * Created: 8-4-2025 11:22:40
 * Author : Tiemen and Tom
 */ 
#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include "display.h"

// HT16K33 I2C address
#define HT16K33_ADDR 0x74

void i2c_init(void) {
	TWSR = 0;
	TWBR = 32; // Set I2C clock to 100kHz with F_CPU = 8MHz and prescaler = 1

	// Enable TWI
	TWCR = (1 << TWEN);
}

void i2c_start(void) {
	TWCR = 0x80 | 0x20 | 0x04; // 0x80 : Clear interrupt flag to start operation, 0x20 : Request START condition, 0x04 : Enable TWI
	while ((TWCR & 0x80) == 0);
}

void i2c_stop(void) {
	TWCR = 0x80 | 0x10 | 0x04; // 0x80 : Clear interrupt flag to start operation, 0x10 : Request STOP condition, 0x04 : Enable TWI
}

void i2c_write(int data) {
	TWDR = data; // Load data into Data Register
	TWCR = 0x80 | 0x04; // 0x80 : Clear interrupt flag to start transmission, 0x04 : Enable TWI
	while ((TWCR & 0x80) == 0);
}

void init_ht16k33(void) {
	i2c_init();
	_delay_ms(10);  // Small delay to ensure i2c is enabled
	
	i2c_start();
	i2c_write((HT16K33_ADDR << 1) | 0);
	i2c_write(0x21); // Turn on oscillator
	i2c_stop();
	_delay_ms(10);
	
	i2c_start();
	i2c_write((HT16K33_ADDR << 1) | 0);
	i2c_write(0xA0); // HT16K33 pins all output (default)
	i2c_stop();
	_delay_ms(10);

	i2c_start();
	i2c_write((HT16K33_ADDR << 1) | 0);
	i2c_write(0x81); // Display on, no blink
	i2c_stop();
	_delay_ms(10);

	i2c_start();
	i2c_write((HT16K33_ADDR << 1) | 0);
	i2c_write(0xEF); // Brightness: max
	i2c_stop();
	_delay_ms(10);
	
	clear_matrix();
}

void clear_matrix(void) {
	for (int row = 0; row < 8; row++) {
		int addr = row * 2; //adresses go from 0x00 to 0x0F, and take 1 numbers. 8 * 2 = 16

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