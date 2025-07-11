#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include "lcd.h"
#include "eeprom.h"
#include <stdlib.h>
#include "dac_8bit.h"
#include "adc.h"
#include <util/delay.h>
#include "fir_filter.h"
#define txAmos 9600
#define freqCorte 1000

#define botaoS1 1
#define botaoS2 2
#define botaoS3 3
#define botaoUP botaoS2
#define botaoDW botaoS1
#define botaoM botaoS3



//declaracao funcoes
void setButtons();
void setInterruptions();
void coefControl();
void printStart();
void printCoef();
ISR(PCINT1_vect);
void setup();
void loop();
volatile int16_t coefs1k[17] = { //FC = 1kHz.
  -1200, -800, 400, 1800, 3000, 3800, 3800, 3000,
  1800, 400, -800, -1200, -1600, -1600, -1200, -800
};
volatile float coefsfloat[17] = {
    0.00806963, 0.01380291, 0.0298653,  0.05459908, 0.08363845, 0.11094114,
 0.13037733, 0.13741233, 0.13037733, 0.11094114, 0.08363845, 0.05459908,
 0.0298653,  0.01380291, 0.00806963
};
//Para FC = 2khz
volatile int16_t coefs[17] = {//FC = 10 Hz, fs=500
11 , 17 , 34 , 59  ,86 ,111 ,128 ,134 ,128 ,111  ,86  ,59  ,34  ,17  ,11,0};
volatile uint8_t coefIndex = 16;
char idex[2];
char strCoef[10];

void setup(){
    setButtons();
    setInterruptions();
    lcd_init();
    loadEEPROM(coefs);
    printStart();
    DAC_Init();
    ADC_Init();
}

void loop(){
    while(1){
    DAC_Write(fir_lowpass(ADC_Read(), coefs));
    }
}

int main(){
    setup();
    loop();
    return 0;
}

void setButtons(){
    DDRC = 0x00;
}

void setInterruptions(){
    PCICR = (1<<PCIE1);
    PCMSK1 = (1<<PCINT9) | (1<<PCINT10) | (1<<PCINT11);
    sei();
}



void printStart(){
    lcd_clear();
    lcd_goto(0,4);
    lcd_print("ELE-3717");
    lcd_goto(1,3);
    lcd_print("FILTRO FIR");
}

void printCoef(){
    lcd_clear();
    lcd_goto(0,4);
    lcd_print("COEFICIENTE");
    itoa(coefIndex, idex, 10);
    itoa(coefs[coefIndex], strCoef, 10);
    lcd_goto(1,0);
    lcd_print("C");
    lcd_print(idex);
    lcd_print(": ");
    lcd_print(strCoef);
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

        if(coefs[coefIndex] <= 32767) coefs[coefIndex]++;
        printCoef();
    }
    else if(~PINC&(1<<botaoDW)){
        if(coefs[coefIndex] >= -32768) coefs[coefIndex]--;
        printCoef();
    }
    saveEEPROM(coefs);
}
