#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include "lcd.h"
#include <stdlib.h>
#include "dac_8bit.h"
#include "adc.h"
#include <util/delay.h>

#define botaoS1 1
#define botaoS2 2
#define botaoS3 3
#define botaoUP botaoS2
#define botaoDW botaoS1
#define botaoM botaoS3

void setButtons();
void setInterruptions();
ISR(PCINT1_vect);

void setup(){
    setButtons();
    setInterruptions();
    lcd_init();
    //loadEEPROM(coefs);
    printStart();
    DAC_Init();
    ADC_Init();
}

ISR(PCINT1_vect){
    if(~PINC&(1<<botaoM)){
        if(coefIndex == 16) coefIndex =0;
        else coefIndex++;
        if(coefIndex == 16) printStart();
        else printCoef();
        coefs[16] = 0;
    }
    else if(~PINC&(1<<botaoUP)){
        //ver limites
        if(coefs[coefIndex] != 32767) coefs[coefIndex]++;
        printCoef();
    }
    else if(~PINC&(1<<botaoDW)){
        if(coefs[coefIndex] != -32768) coefs[coefIndex]--;
        printCoef();
    }
    saveEEPROM(coefs);
}
