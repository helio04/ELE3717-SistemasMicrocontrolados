#ifndef DAC_8BIT_H
#define DAC_8BIT_H

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

void DAC_Init(void);


void DAC_Write(uint8_t value);
void DAC_Write_Reversed(uint8_t value);
#endif
