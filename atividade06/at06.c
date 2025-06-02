#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include "lcd.h"
#include "eeprom.h"
#include <stdlib.h>

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
volatile int16_t coefs[16] = {50 , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
volatile uint8_t coefIndex = 16;
char idex[2];
char strCoef[10];
uint8_t prevCase = 16;
volatile uint8_t update = 1;
uint8_t prevValue = 0;

void setup(){
    setButtons();
    setInterruptions();
    lcd_init();
    loadEEPROM(coefs);
    printStart();

}

void loop(){
    while(1){
    //coefControl();
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

void coefControl(){
    switch (coefIndex)
    {
    case 16:
       if(prevCase!=16){
            printStart();
            prevCase = 16;
        }
        break;

    case 0:
        if(prevCase != 0 && update ==1){ //&& prevValue != coefs[0]){
            printCoef();
            prevCase = 0;
            update =0 ;
        }
    break;
    case 1:
        //if(prevCase != 1 && update ==1){ //&& prevValue != coefs[1]){
            printCoef();
            prevCase = 1;
            update=0;
        //}
    break;
    case 2:
        if(prevCase != 2 && prevValue != coefs[2]){
            printCoef();
            prevCase = 2;
            prevValue = coefs[2];
        }
    break;
    case 3:
        if(prevCase != 3 && prevValue != coefs[3]){
            printCoef();
            prevCase = 3;
            prevValue = coefs[3];
        }
    break;
    case 4:
        if(prevCase != 4 && prevValue != coefs[4]){
            printCoef();
            prevCase = 4;
            prevValue = coefs[4];
        }
    break;
    case 5:
        if(prevCase != 5 && prevValue != coefs[5]){
            printCoef();
            prevCase = 5;
            prevValue = coefs[5];
        }
    break;
    case 6:
        if(prevCase != 6 && prevValue != coefs[6]){
            printCoef();
            prevCase = 6;
            prevValue = coefs[6];
        }
    break;
    case 7:
        if(prevCase != 7 && prevValue != coefs[7]){
            printCoef();
            prevCase = 7;
            prevValue = coefs[7];
        }
    break;
    case 8:
        if(prevCase != 8 && prevValue != coefs[8]){
            printCoef();
            prevCase = 8;
            prevValue = coefs[8];
        }
    break;
    case 9:
        if(prevCase != 9 && prevValue != coefs[9]){
            printCoef();
            prevCase = 9;
            prevValue = coefs[9];
        }
    break;
    case 10:
        if(prevCase != 10 && prevValue != coefs[10]){
            printCoef();
            prevCase = 10;
            prevValue = coefs[10];
        }
    break;
    case 11:
        if(prevCase != 11 && prevValue != coefs[11]){
            printCoef();
            prevCase = 11;
            prevValue = coefs[11];
        }
    break;
    case 12:
        if(prevCase != 12 && prevValue != coefs[12]){
            printCoef();
            prevCase = 12;
            prevValue = coefs[12];
        }
    break;
    case 13:
        if(prevCase != 13 && prevValue != coefs[13]){
            printCoef();
            prevCase = 13;
            prevValue = coefs[13];
        }
    break;
    case 14:
        if(prevCase != 14 && prevValue != coefs[14]){
            printCoef();
            prevCase = 14;
            prevValue = coefs[14];
        }
    break;
    case 15:
        if(prevCase != 15 && prevValue != coefs[15]){
            printCoef();
            prevCase = 15;
            prevValue = coefs[15];
        }
    break;


    default:
        
        break;
    }

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
        if(coefs[coefIndex] != 32000) coefs[coefIndex]++;
        printCoef();
    }
    else if(~PINC&(1<<botaoDW)){
        if(coefs[coefIndex] != -2000) coefs[coefIndex]--;
        printCoef();
    }
    saveEEPROM(coefs);
}
