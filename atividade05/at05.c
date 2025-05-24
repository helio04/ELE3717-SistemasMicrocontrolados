#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include "lcd.h"
#include <stdlib.h>
#include "matriz.h"
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
#define caseDefault 4
#define caseReset 5
#define neutralJoyX 501
#define neutralJoyY 510
#define deadzone 30
#define minutoTimer 120 ///120


//def vars
int prevX, prevY;
 int posX[MaxsizeVector];
 int posY[MaxsizeVector];

  volatile short int marX[10];
  volatile short int marY[10];
  volatile short int tailX = 0;
  volatile short int tailY = 0;
  volatile short int iter=0;
  volatile short int sizeVector = 1;
  volatile short int prevCase= caseXup;
  volatile long int gameTime= 0;
  volatile int delayGame = 500;
char  printXvalues[7][5];
char  printYvalues[7][5];
//def funcoes
int main();
void setup();
void changePos(int caseMove);
void shiftvector(volatile   short int vector[],   short int newPos);
  int readADCX();
  int readADCY();
void setADC();
void setPins();
void mvSnake();
void loop();
void customDelay(  int time);
void writeLCDteste();
void writeLCDADC();
void shiftvectorsupreme(int vector[], int newPos);
void checkReset( int *casemove1);
void resetGame();
void writeMatrix();

void setup(){
    posX[0] = 3;
    posY[0] = 4;
    setPins();
    setADC();
    lcd_init();
    spi_init();
    max7219_init();
    delayGame = 500;
}
void setPins(){
    DDRC = 0x0;
    DDRB = (1<<PB5) | (1<<PB3) | (1<<PB2);
    DDRD = 0xFF;
}

  int readADCX(){
    ADMUX = (ADMUX & 0xF0) | (1<<MUX2);
    ADCSRA |= (1 << ADSC);              
    while (ADCSRA & (1 << ADSC));      
    return ADC; 
}
  int readADCY(){
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

void changePos(int caseMove){
    checkReset(&caseMove);
    prevX = posX[0];
    prevY = posY[0];
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
        case caseDefault:
        changePos(prevCase);
        break;
        case caseReset:
        resetGame();
        break;
        default:
        changePos(prevCase);
        break;
    }
}


void shiftvector(volatile   short int vector[],   short int newPos){
    for(  short int i = sizeVector-1; i>0; i--){
        vector[i] = vector[i-1];
    }
    vector[0] = newPos;
}

void shiftvectorsupreme(int vector[],  int newPos){
      short int aux[MaxsizeVector];
    for (  short int i =0; i<MaxsizeVector;i++){
        aux[i] = vector[i];
    }
    vector[0] = newPos;
    for(  short int i = 1; i<sizeVector;i++){
            vector[i] = aux[i-1];
        }
       
    
}


void mvSnake(){
    if(readADCX()>neutralJoyX+deadzone) changePos(caseXup);
    else if(readADCX()<neutralJoyX-deadzone) changePos(caseXdw);
    else if(readADCY()>neutralJoyY+deadzone) changePos(caseYup);
    else if(readADCY()<neutralJoyY-deadzone) changePos(caseYdw);
    else changePos(caseDefault);
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
    writeMatrix();
    
}
}

void customDelay(  int time){
    for(  short int i = 0; i<time;i++){
        _delay_ms(1);
    }
}


void writeLCDteste(){
    
   for(  int a = 0; a<MaxsizeVector; ++a){
        itoa(posY[a], printYvalues[a], 10);
   }
   for(int a= 0; a<MaxsizeVector;a++){
    itoa(posX[a], printXvalues[a], 10);
   }
    
    lcd_goto(0,0);
    lcd_print(printXvalues[0]);
    lcd_goto(1,0);
    lcd_print(printYvalues[0]);
    for(  int i = 1; i<=7;i++){
    
        lcd_goto(0, i*3);
        lcd_print(printXvalues[i]);
        lcd_goto(1, i*3);
        lcd_print(printYvalues[i]);
    }
}
void writeLCDADC(){
    itoa(readADCX(), printXvalues[0], 10);
    itoa(readADCY(),printYvalues[0],10);
    lcd_goto(0,0);
    lcd_print(printXvalues[0]);
    lcd_goto(1,0);
    lcd_print(printYvalues[0]);
}

void checkReset( int *casemove1){
    if(posX[0]<0 || posX[0]>7 ||posY[0]<0 || posY[0]>7){
        *casemove1 = caseReset;
    }
    for(  short int i=1; i<MaxsizeVector;i++){
        if(posX[0]==posX[i] && posY[0]==posY[i]){
            *casemove1 = caseReset;
        }
    }
}

void resetGame(){
    for(int i =0; i<MaxsizeVector; i++){
        posX[i] = 0;
        posY[i] = 0;
    }
        posX[0] = 3;
        posY[0] = 4;
        delayGame = 500;
        max7219_clear();
}
void writeMatrix(){
    max7219_draw_dot_trail(posX, posY, sizeVector);
}


