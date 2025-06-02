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
#define deadzone 250
#define minutoTimer 10 ///120


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
void shiftvectortranscendent(int vectorshifted[], int newPos, int vectorsec[]);
void shiftvectordivine(int vectormain[], int newPos, int vectorsec[]);
void checkReset();
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
    prevX = posX[0];
    prevY = posY[0];
    switch(caseMove){
        case caseXup:
        //shiftvectorsupreme(posX, posX[0]+1);
        shiftvectordivine(posX, posX[0]+1, posY);
        prevCase = caseXup;
        break;
        case caseXdw:
        //shiftvectorsupreme(posX, posX[0]-1);
        shiftvectordivine(posX, posX[0]-1, posY);
        prevCase = caseXdw;
        
        break;
        case caseYup:
        //shiftvectorsupreme(posY, posY[0]+1);
        shiftvectordivine(posY, posY[0]+1, posX);
        prevCase = caseYup;
        break;
        case caseYdw:
        //shiftvectorsupreme(posY, posY[0]-1);
        shiftvectordivine(posY, posY[0]-1, posX);
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
    checkReset();

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
    
    for(  short int i = 1; i<sizeVector;i++){
            vector[i] = aux[i-1];
        }
       vector[0] = newPos;
    
}
void shiftvectortranscendent(int vectorshifted[], int newPos, int vectorsec[]){
     int aux[MaxsizeVector];
     int aux2[MaxsizeVector];
    for (int i =0; i<MaxsizeVector;i++){
        aux[i] = vectorshifted[i];
    }
    for(int i =0; i<MaxsizeVector;i++){
        aux2[i] = vectorsec[i];
    }
    vectorshifted[0] = newPos;
    vectorsec[0] = vectorsec[0];
    for(int i = 1; i<sizeVector;i++){
            vectorshifted[i] = aux[i-1];
        }
    for(int i = 1; i<sizeVector;i++){
            vectorsec[i] = aux2[i-1];
        }
}
void shiftvectordivine(int vectormain[], int newPos, int vectorsec[]){
     for(  short int i = sizeVector-1; i>0; i--){
        vectormain[i] = vectormain[i-1];
        vectorsec[i] = vectorsec[i-1];
    }
    vectormain[0] = newPos;
}

void mvSnake(){
    if(readADCX()>neutralJoyX+deadzone) changePos(caseXup);
    else if(readADCX()<neutralJoyX-deadzone) changePos(caseXdw);
    else if(readADCY()>neutralJoyY+deadzone) changePos(caseYdw);
    else if(readADCY()<neutralJoyY-deadzone) changePos(caseYup);
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
        if(sizeVector<7) sizeVector++;
        delayGame -= 50;
    }
    writeLCDteste();
   //writeLCDADC(); 
   writeMatrix();
   customDelay(delayGame);

    
}
}

void customDelay(  int time){
    for( int i = 0; i<time;i++){
        _delay_ms(1);
    }
}


void writeLCDteste(){
    
   for(  int a = 0; a<MaxsizeVector; a++){
        itoa(posY[a], printYvalues[a], 10);
   }
   for(int a= 0; a<MaxsizeVector;a++){
    itoa(posX[a], printXvalues[a], 10);
   }
    
    lcd_goto(0,0);
    lcd_print(printXvalues[0]);
    lcd_print(" ");
    lcd_print(printXvalues[1]);
    lcd_print(" ");
    lcd_print(printXvalues[2]);
    lcd_print(" ");
    lcd_print(printXvalues[4]);
    lcd_print(" ");
    lcd_print(printXvalues[5]);
    lcd_print(" ");
    lcd_print(printXvalues[6]);
    lcd_print(" ");
    lcd_print(printXvalues[7]);
    lcd_goto(1,0);
    lcd_print(printYvalues[0]);
    lcd_print(" ");
    lcd_print(printYvalues[1]);
    lcd_print(" ");
    lcd_print(printYvalues[2]);
    lcd_print(" ");
    lcd_print(printYvalues[4]);
    lcd_print(" ");
    lcd_print(printYvalues[5]);
    lcd_print(" ");
    lcd_print(printYvalues[6]);
    lcd_print(" ");
    lcd_print(printYvalues[7]);
}
void writeLCDADC(){
    char sizeofserpent[3];
    char delayofthegame[4];
    itoa(readADCX(), printXvalues[0], 10);
    itoa(readADCY(),printYvalues[0],10);
    lcd_goto(0,0);
    lcd_print(printXvalues[0]);
    lcd_goto(1,0);
    lcd_print(printYvalues[0]);
    itoa(sizeVector, sizeofserpent, 10);
    lcd_goto(1, 5);
    lcd_print(sizeofserpent);
    itoa(delayGame, delayofthegame, 10);
    lcd_goto(0,5);
    lcd_print(delayofthegame);
}

void checkReset(){
    if(posX[0]<0 || posX[0]>8 ||posY[0]<0 || posY[0]>8){
        resetGame();
    }
    for(  short int i=1; i<MaxsizeVector;i++){
        if(posX[0]==posX[i] && posY[0]==posY[i]){
            //resetGame();
            lcd_goto(1,7);
            lcd_print("R");
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
        sizeVector = 1;
        max7219_clear();
        lcd_clear();
        
}
void writeMatrix(){
   // max7219_draw_dot_trail(posX, posY, sizeVector);
    //max7219_draw_dot(posX[0], posY[0]);
    convertM(posX,posY);
}


