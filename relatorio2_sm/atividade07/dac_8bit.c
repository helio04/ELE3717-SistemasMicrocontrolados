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


void DAC_Write_Reversed(uint8_t value) {
  /*
    Reversed Bit Mapping:
    D13 (PB5) = bit7 (MSB)
    D12 (PB4) = bit6
    D11 (PB3) = bit5
    D10 (PB2) = bit4
    D9  (PB1) = bit3
    D8  (PB0) = bit2
    A4  (PC4) = bit1
    A5  (PC5) = bit0 (LSB)
  */


  PORTC = (PORTC & 0xCF)  | ((value << 4) & 0x30);


  PORTB = (PORTB & 0xC0)   | ((value >> 2) & 0x3F);
}


