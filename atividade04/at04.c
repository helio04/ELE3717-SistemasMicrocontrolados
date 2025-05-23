#define F_CPU 16000000UL

#include <avr/interrupt.h>

#include "lcd.h"
#include <stdlib.h>


#define botao1 1
#define botao2 2
#define botao3 3
#define botaoup botao3
#define botaodown botao1
#define botaoM botao2
#define pinoRED 3
#define pinoGREEN 2
#define pinoBLUE 1
#define desvioMax 4

#define addrRED 0x02
#define addrGREEN 0x08
#define addrBLUE 0x14
#define startSpotRed 0
#define startSpotGreen 5
#define startSpotBlue 12
#define pointerRed 3
#define pointerGreen 8
#define pointerBlue 15
#define casoRED 1<<1
#define casoGREEN 1<<2
#define casoBLUE 1<<3

volatile unsigned int valorred = 0;
volatile unsigned int valorblue = 0;
volatile unsigned int valorgreen = 0;
volatile unsigned int controle = 0x01;
volatile unsigned int curRed = 0;
volatile unsigned int curBlue= 0;
volatile unsigned int curGreen=0;
 char redstr[5];
 char greenstr[5];
 char bluestr[5];
volatile unsigned short int contador = 0;
volatile unsigned short int passo = 1;

///dec functions
void setup();
void setPinos();
void setInterruptions();
void setup();
void updateValuesRGB(unsigned int red1, unsigned int green1, unsigned int blue1);
void loop();
void incRed();
void decRed();
void incGreen();
void decGreen();
void incBlue();
void decBlue();
void saveEEPROM(unsigned int valorREDEEPROM, unsigned int valorGREENEEPROM, unsigned int valorBLUEEEPROM);
void loadEEPROM();
void writeEEPROM(unsigned int addr, uint8_t dados);
uint8_t readEEPROM(unsigned int addr);
void setTimers();

ISR(PCINT1_vect);
void updateREDLCD();
void updateGREENLCD();
void updateBLUELCD();
void pointRED();
void pointGREEN();
void pointBLUE();
void clearpointRED();
void clearpointGREEN();
void clearpointBLUE();
void writeLEDPWM();

void setup(){
    setPinos();
    setInterruptions();
    loadEEPROM();
    lcd_init();
    lcd_goto(0,0);
    lcd_print("RED  GREEN  BLUE");
    updateREDLCD();
    updateGREENLCD();
    updateBLUELCD();
    setTimers();
    updateValuesRGB(curRed,curGreen,curBlue);
}

void setPinos(){
    DDRD = 0xFF;
    DDRB |= (1<<pinoRED) | (1<<pinoGREEN) | (1<<pinoBLUE);
}

void setInterruptions(){
    PCICR = (1<<PCIE1);
    PCMSK1 = (1<<PCINT9) | (1<<PCINT10) | (1<<PCINT11);
    sei();
}

void updateValuesRGB(unsigned int red1, unsigned int green1, unsigned int blue1){
    OCR1A = red1;
    OCR1B = green1;
    OCR2A = blue1;
}


void writeLed(){
    for(int i =0; i<valorred;i++){
        PORTB |= (1<<pinoRED);
    }
    PORTB &= ~(1<<pinoRED);
    for(int i=0; i<valorgreen; i++){
        PORTB |= (1<<pinoGREEN);
    }
    PORTB &= ~(1<<pinoGREEN);
    for(int i=0; i<valorblue; i++){
        PORTB |= (1<<pinoBLUE);
    }
    PORTB &= ~(1<<pinoBLUE);


}

void incRed(){
    if(curRed+passo<=255){
    curRed+=passo;}
}
void decRed(){
    if(curRed-passo>=0) {
    curRed-=passo;}
}
void incGreen(){
    if(curGreen+passo<=255) {
    curGreen+=passo;
    }
}
void decGreen(){
    if(curGreen-passo>=0){
    curGreen-=passo;
    }
}
void incBlue(){
    if(curBlue+passo<=255) {
    curBlue+=passo;
    }
}
void decBlue(){
    if(curBlue-passo>=0)
    {curBlue-=passo;}
}

ISR(PCINT1_vect){
    contador = 0;
    passo = 1;
    if(~PINC & (1<<botaoup)){
        while(~PINC & (1<<botaoup)){
        switch (controle)
        {
        case (casoRED):  //RED
            incRed();
            updateREDLCD();
            break;
        case (casoGREEN): //GREEN
            incGreen();
            updateGREENLCD();
        break;
        case (casoBLUE): //BLUE
            incBlue();
            updateBLUELCD();
        break;
        default:
            break;
        }
    updateValuesRGB(curRed,curGreen,curBlue);
    contador++;
    if(contador >=5){
        passo = 5;
    }
    _delay_ms(500);
    }}
    else if(~PINC & (1<<botaodown)){
        while(~PINC & (1<<botaodown)){
        switch (controle)
        {
        case (casoRED):  //RED
            decRed();
            updateREDLCD();
            break;
        case (casoGREEN): //GREEN
            decGreen();
            updateGREENLCD();
        break;
        case (casoBLUE): //BLUE
            decBlue();
            updateBLUELCD();
        break;
        default:
            break;
            }
    updateValuesRGB(curRed,curGreen,curBlue);
    contador++;
    if(contador >=5){
        passo = 5;
    }
    _delay_ms(500);
    }}
    else if(~PINC & (1<<botaoM)){
        if(controle == (1<<desvioMax)){
            controle = 0x01;
            clearpointBLUE();
            clearpointRED();
            clearpointGREEN();
        }
        else{controle = (controle<<1);}
        switch(controle){
            case (casoRED):
            clearpointBLUE();
            clearpointGREEN();
            pointRED();
            break;
            case(casoGREEN):
            clearpointRED();
            clearpointBLUE();
            pointGREEN();
            break;
            case(casoBLUE):
            clearpointRED();
            clearpointGREEN();
            pointBLUE();
            break;
            case(1<<0):
            clearpointBLUE();
            clearpointRED();
            clearpointGREEN();
            break;
            case(1<<desvioMax):
            clearpointBLUE();
            clearpointRED();
            clearpointGREEN();
            break;
        }

    }
   
    
    saveEEPROM(curRed,curGreen,curBlue);
    
}


int main(){
    setup();
    loop();
    return 0;
}

void loop(){
    while(1){}
}

void writeEEPROM(unsigned int addr, uint8_t dados){
    while (EECR & (1 << EEPE));
    EEAR = addr;
    EEDR = dados;
    EECR |= (1 << EEMPE);
    EECR |= (1 << EEPE);
}
uint8_t readEEPROM(unsigned int addr){
    while (EECR & (1 << EEPE));

    EEAR = addr;

    EECR |= (1 << EERE);
return EEDR;
}
void saveEEPROM(unsigned int valorREDEEPROM, unsigned int valorGREENEEPROM, unsigned int valorBLUEEEPROM){
    writeEEPROM(addrRED, valorREDEEPROM);
    writeEEPROM(addrGREEN,valorGREENEEPROM);
    writeEEPROM(addrBLUE, valorBLUEEEPROM);
}
void loadEEPROM(){
    curRed = readEEPROM(addrRED);
    curGreen = readEEPROM(addrGREEN);
    curBlue = readEEPROM(addrBLUE);
}

void updateREDLCD(){
    
    lcd_goto(1,startSpotRed);
    itoa(curRed,redstr,10);
    //if(curRed <cem) redstr[2] = ' ';
    //if(curRed<dez ) redstr[1] = ' ';
    lcd_print(redstr);
}
void updateGREENLCD(){
    lcd_goto(1,startSpotGreen);
    itoa(curGreen, greenstr, 10);
    //if(curGreen<cem) greenstr[2] = ' ';
    //if (curGreen<dez) greenstr[1] = ' ';
    
    lcd_print(greenstr);
}

void updateBLUELCD(){
    
    lcd_goto(1,startSpotBlue);
    itoa(curBlue, bluestr, 10);
    //if(curBlue<cem) bluestr[2] = ' ';
    //if(curBlue<dez) bluestr[1] = ' ';
    lcd_print(bluestr);
}

void pointRED(){
    lcd_goto(1,pointerRed);
    lcd_print("*");
}

void pointBLUE(){
    lcd_goto(1, pointerBlue);
    lcd_print("*");
}

void pointGREEN(){
    lcd_goto(1, pointerGreen);
    lcd_print("*");
}

void clearpointRED(){
    lcd_goto(1,pointerRed);
    lcd_print("_");
}

void clearpointGREEN(){
    lcd_goto(1, pointerGreen);
    lcd_print("_");
}
void clearpointBLUE(){
    lcd_goto(1, pointerBlue);
    lcd_print("_");
}

void writeLEDPWM(){
    for(int i =0; i<valorred;i++){
        PORTB |= (1<<pinoRED);
        _delay_us(50);
    }
    PORTB &= ~(1<<pinoRED);
    for(int i=0; i<valorgreen; i++){
        PORTB |= (1<<pinoGREEN);
        _delay_us(50);
    }
    PORTB &= ~(1<<pinoGREEN);
    for(int i=0; i<valorblue; i++){
        PORTB |= (1<<pinoBLUE);
        _delay_us(501);
    }
    PORTB &= ~(1<<pinoBLUE);
}

void setTimers() {
    //8-bit FAST PWM e sem preescala
    TCCR1A |= (1<< COM1A1) |  (1<<COM1B1) | (1<<WGM10);
    TCCR1B |= (1<<WGM12) |(1<<CS11);
    TCCR2A |= (1<<COM2A1) | (1<<WGM21) | (1<<WGM20);
    TCCR2B |= (1<<CS21);
    OCR1A = curRed;
    OCR1B = curGreen;
    OCR2A = curBlue;
}
