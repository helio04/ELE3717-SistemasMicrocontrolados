#include "lcd2.h"

void cmd_LCD(unsigned char c, char cd){
    if(cd==0) clr_bit(CONTR_LCD,RS);
    else set_bit(CONTR_LCD,RS);
    #if (nibble_dados) PORTD = (PORTD & 0x0F)|(0xF0 & c);
    #else PORTD = (PORTD & 0xF0)|(c>>4);
    #endif
    pulso_enable();
    #if(nibble_dados) PORTD = (PORTD & 0x0F) | (0xF0 & (c<<4));
    #else PORTD = (PORTD & 0xF0) | (0x0F & c);
    #endif
    pulso_enable();
    if((cd==0) && (c<4)) _delay_ms(2);
}

void inic_LCD_4bits(){
    clr_bit(CONTR_LCD,RS);
    clr_bit(CONTR_LCD,E);
    _delay_ms(20);
    #if (nibble_dados) PORTD = (PORTD & 0x0F) | 0x30;
    #else PORTD = (PORTD & 0xF0) | 0x03;
    #endif
    pulso_enable();
    _delay_ms(5);
    pulso_enable();
    _delay_us(200);
    pulso_enable();
    #if(nibble_dados) PORTD = (PORTD & 0x0F) | 0x20;
    #else PORTD = (PORTD & 0xF0) | 0x02;
    #endif
    pulso_enable();
    cmd_LCD(0x28,0);
    cmd_LCD(0x08,0);
    cmd_LCD(0x01,0);
    cmd_LCD(0x0C,0);
    cmd_LCD(0x80,0);
}

void escreve_lcd(char* c){
    for (; *c!=0;c++) cmd_LCD(*c,1);
}