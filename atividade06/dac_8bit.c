#include "dac_8bit.h"
#include <avr/io.h>


void DAC_Init(void) {
    
    DDRC |= (1 << DDC4) | (1 << DDC5);
    
    
    DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | 
            (1 << DDB3) | (1 << DDB4) | (1 << DDB5);
    
    
    DAC_Write(0);
}

void DAC_Write(uint8_t value) {
  
  PORTB = (PORTB & 0b11000000) | ((value >> 2) & 0b00111111);

  
  PORTC = (PORTC & 0b11001111)
         | ((value << 3) & 0b00110000); 
}

