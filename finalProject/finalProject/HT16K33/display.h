/*
 * display.h
 *
 * Created: 8-4-2025 11:22:57
 *  Author: tieme
 */ 

#include <avr/io.h>

#ifndef DISPLAY_H_
#define DISPLAY_H_

void i2c_init(void);
void ht16k33_init(void);
void clear_matrix(void);
void set_pixel(uint8_t row, uint8_t col, uint8_t on);


#endif /* DISPLAY_H_ */