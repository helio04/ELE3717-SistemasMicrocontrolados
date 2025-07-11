#include "adc.h"
#include <avr/io.h>
#include <util/delay.h>

void ADC_Init(void) {
    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t ADC_Read(void) {
    ADMUX = (ADMUX & 0xF0) | (0 & 0x0F);
    ADCSRA |= (1 << ADSC);  
    while (ADCSRA & (1 << ADSC));
    return ADC;
}