/*
 * display.h
 *
 * Created: 8-4-2025 11:22:57
 * Author : Tiemen and Tom
 */ 

#include <avr/io.h>

#ifndef DISPLAY_H_
#define DISPLAY_H_

//set up I2C and display.
void init_ht16k33(void);

void clear_matrix(void);

void setrow(int row, int vallue);

#endif