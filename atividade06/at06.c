#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include "lcd.h"
#include "eeprom.h"
#include <stdlib.h>
#include "dac_8bit.h"
#include "adc.h"
#include <util/delay.h>
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
volatile int16_t coefs[17] = { //FC = 1kHz.
  -1200, -800, 400, 1800, 3000, 3800, 3800, 3000,
  1800, 400, -800, -1200, -1600, -1600, -1200, -800
};
volatile int16_t coefs2k[17] = {
74, 524,328, -2372, -3790,  5174, 23078, 32767, 23078,  
5174, -3790, -2372,   328,   524,  74
};
//Para FC = 2khz
volatile uint8_t coefIndex = 16;
char idex[2];
char strCoef[10];

void setup(){
    setButtons();
    setInterruptions();
    lcd_init();
    //loadEEPROM(coefs);
    printStart();
    DAC_Init();
    ADC_Init();
}
char testeitoa[6];
  //if(cont<255) cont++;
    //lcd_goto(0,0);
    //itoa(cont, testeitoa, 10);
    //lcd_print(testeitoa);
    //_delay_ms(500);
void loop(){
    uint8_t cont =0;
    while(1){
    int8_t value = (int8_t) (ADC_Read()>>2);
    DAC_Write(cont);
    //DAC_Write_Reversed(cont);
  cont +=10;
  if(cont>250) cont=0;
    //lcd_goto(0,0);
    //itoa(cont, testeitoa, 10);
    //lcd_print(testeitoa);
    //_delay_ms(500);
  _delay_ms(1000);
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
