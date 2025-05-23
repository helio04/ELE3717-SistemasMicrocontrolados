#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include "lcd.h"
#include <stdlib.h>

//def
#define joyX PC4
#define joyY PC5
#define clockM PB5
#define LoadM PB2
#define DINM PB3
#define MaxsizeVector 8
#define caseXup 0
#define caseXdw 1
#define caseYup 2
#define caseYdw 3
#define neutralJoy 512
#define deadzone 30
#define minutoTimer 10 ///120


//def vars
unsigned volatile short int posX[MaxsizeVector];
unsigned volatile short int posY[MaxsizeVector];
unsigned volatile short int marX[10];
unsigned volatile short int marY[10];
unsigned volatile short int tailX = 0;
unsigned volatile short int tailY = 0;
unsigned volatile short int iter=0;
unsigned volatile short int sizeVector = 1;
unsigned volatile short int prevCase= caseXup;
unsigned volatile long int gameTime= 0;
unsigned volatile int delayGame = 500;
char  printXvalues[7][5];
char  printYvalues[7][5];
//def funcoes
int main();
void setup();
void changePos(unsigned short int caseMove);
void shiftvector(volatile unsigned short int vector[], unsigned short int newPos);
unsigned int readADCX();
unsigned int readADCY();
void setADC();
void setPins();
void mvSnake();
void loop();
void customDelay(unsigned int time);
void writeLCDteste();
void setup(){
    setPins();
    setADC();
    lcd_init();
    delayGame = 500;
}
void setPins(){
    DDRC = 0x0;
    DDRB = (1<<PB5) | (1<<PB3) | (1<<PB2);
    DDRD = 0xFF;
}

unsigned int readADCX(){
    ADMUX = (ADMUX & 0xF0) | (1<<MUX2);
    ADCSRA |= (1 << ADSC);              
    while (ADCSRA & (1 << ADSC));      
    return ADC; 
}
unsigned int readADCY(){
    ADMUX =(ADMUX & 0xF0) |(1<<MUX2) | (1<<MUX0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    ADCSRA |= (1 << ADSC);              
    while (ADCSRA & (1 << ADSC));      
    return ADC;
}
void setADC(){
    ADMUX = (1<<REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

void changePos(unsigned short int caseMove){

    switch(caseMove){
        case caseXup:
        shiftvectorsupreme(posX, posX[0]+1);
        prevCase = caseXup;
        break;
        case caseXdw:
        shiftvectorsupreme(posX, posX[0]-1);
        prevCase = caseXdw;
        break;
        case caseYup:
        shiftvectorsupreme(posY, posY[0]+1);
        prevCase = caseYup;
        break;
        case caseYdw:
        shiftvectorsupreme(posY, posY[0]-1);
        prevCase = caseYdw;
        break;
        default:
        changePos(prevCase);
        break;
    }
}


void shiftvector(volatile unsigned short int vector[], unsigned short int newPos){
    for(unsigned short int i = sizeVector-1; i>0; i--){
        vector[i] = vector[i-1];
    }
    vector[0] = newPos;
}

void shiftvectorsupreme(volatile unsigned short int vector[], unsigned short int newPos){
    unsigned short int aux[MaxsizeVector];
    for (unsigned short int i =0; i<MaxsizeVector;i++){
        aux[i] = vector[i];
    }
    vector[0] = newPos;
    for(unsigned short int i = 1; i<sizeVector;i++){
            vector[i] = aux[i-1];
        }
       
    
}


void mvSnake(){
    if(readADCX()>neutralJoy+deadzone) changePos(caseXup);
    else if(readADCX()<neutralJoy-deadzone) changePos(caseXdw);
    else if(readADCY()>neutralJoy+deadzone) changePos(caseYup);
    else if(readADCY()<neutralJoy-deadzone) changePos(caseYdw);
    else changePos(10);
}

int main(){
   setup();
    loop();
    return 0;
}

//char teste[5];
//char teste2[5];
void loop(){
    while(1){
     mvSnake();
    customDelay(delayGame);
    gameTime++;
   /* itoa(gameTime, teste, 10);
    lcd_goto(0,0);
    lcd_print(teste);
    itoa(delayGame, teste2, 10);
    lcd_goto(1,0);
    lcd_print(teste2);*/
    if(gameTime>minutoTimer && delayGame==100) gameTime=0;
    if(gameTime>=minutoTimer && delayGame>100){
        gameTime = 0;
        if(sizeVector<=7) sizeVector++;
        delayGame -= 50;
    }
    writeLCDteste();
}
}

void customDelay(unsigned int time){
    for(unsigned short int i = 0; i<time;i++){
        _delay_ms(1);
    }
}


void writeLCDteste(){
    for(unsigned int a = 0; a<MaxsizeVector; a++){
        itoa(posX[a], printXvalues[a], 10);
        itoa(posY[a], printYvalues[a], 10);
    }
    for(unsigned int i = 0; i<=7;i++){
    
        lcd_goto(0, i*3+1);
        lcd_print(printXvalues[i]);
        lcd_goto(1, i*3+1);
        lcd_print(printYvalues[i]);
    }
}

void writeMatrix(){
    
}
