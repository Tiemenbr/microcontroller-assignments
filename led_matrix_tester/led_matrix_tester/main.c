#define F_CPU 8000000UL  // 8 MHz clock (adjust as needed)
#define SCL_CLOCK 100000L // 100kHz I2C speed

#include <avr/io.h>
#include <util/delay.h>

// I2C setup functions (already defined in previous steps)
void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
uint8_t i2c_write(uint8_t data);

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
	/*
	for (uint8_t col = 0; col < 8; col++) {
		i2c_start();
		i2c_write((HT16K33_ADDR << 1) | 0); // Write mode
		i2c_write(col); // Column address (0 to 7)
		i2c_write(0x00); // Clear the column (turn off all pixels)
		i2c_stop();
	}
	_delay_ms(10); // Wait to ensure the matrix is cleared
	*/
	for (uint8_t addr = 0x00; addr <= 0x0F; addr++) {
		i2c_start();
		i2c_write((HT16K33_ADDR << 1) | 0); // Write mode
		i2c_write(addr); // HT16K33 RAM address (0x00 to 0x0F)
		i2c_write(0x00); // Clear all bits in this address
		i2c_stop();
	}
	_delay_ms(10); // Give time to update
}

// Set the right-most pixel for rows 5, 6, 7, 8
void set_bottom_rows(void) {
	/*
	uint8_t pattern = 64;  // 0x80 turns on the right-most pixel (bit 7)

	// Set pixels in rows 5, 6, 7, 8 (corresponds to row 4 to 7 in the matrix)
	for (uint8_t row = 4; row < 8; row++) {
		i2c_start();
		i2c_write((HT16K33_ADDR << 1) | 0);  // Write mode
		i2c_write(row); // Row address (4 to 7 corresponding to rows 5 to 8)
		i2c_write(pattern); // Set the right-most pixel (bit 7)
		i2c_stop();
	}
	*/
	uint8_t addr_even = 0x0E; // Column 7, even rows
	uint8_t addr_odd  = 0x0F; // Column 7, odd rows

	// Turn on bits for rows 4–7
	// Row 4 = bit 4
	// Row 5 = bit 5
	// Row 6 = bit 6
	// Row 7 = bit 7
	uint8_t pattern = 0b11110000; // = 0xF0

	i2c_start();
	i2c_write((HT16K33_ADDR << 1) | 0);
	i2c_write(addr_even);
	i2c_write(pattern); // Set even row bits (4 and 6)
	i2c_stop();

	i2c_start();
	i2c_write((HT16K33_ADDR << 1) | 0);
	i2c_write(addr_odd);
	i2c_write(pattern); // Set odd row bits (5 and 7)
	i2c_stop();

	_delay_ms(10);
}

int main(void) {
	// Initialize the system
	i2c_init();    // Initialize I2C (TWI)
	ht16k33_init(); // Initialize HT16K33 matrix

	// Clear the matrix before displaying anything new
	clear_matrix(); // Turn off all pixels

	// Set the right-most pixel in rows 5, 6, 7, 8
	set_bottom_rows();

	// Main loop (do nothing, let the matrix display the pixels)
	while (1) {
		// Infinite loop to keep the program running
	}

	return 0;
}