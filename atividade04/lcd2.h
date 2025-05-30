#ifndef LCD_H
#define LCD_H
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define set_bit(y,bit) (y|=(1<<bit)) //coloca em 1 o bit x da variável Y
#define clr_bit(y,bit) (y&=~(1<<bit)) //coloca em 0 o bit x da variável Y
#define cpl_bit(y,bit) (y^=(1<<bit)) //troca o estado lógico do bit x da variável Y
#define tst_bit(y,bit) (y&(1<<bit)) //retorna 0 ou 1 conforme leitura do bit


#define DADOS_LCD PORTD//4 bits de dados do LCD no PORTD
#define nibble_dados 1
/*0 para via de dados do LCD nos 4 LSBs do PORT
empregado (Px0-D4, Px1-D5, Px2-D6, Px3-D7), 1 para via de
dados do LCD nos 4 MSBs do PORT empregado (Px4-D4, Px5-D5,
Px6-D6, Px7-D7) */
#define CONTR_LCD PORTD//PORT com os pinos de controle do LCD (pino R/W em 0).
#define E PD3 //pino de habilitação do LCD (enable)
#define RS PD2 //pino para informar se o dado é uma instrução ou caractere

//número de digitos individuais para a conversão por ident_num()
/*48 se ident_num() deve retornar um número no formato ASCII (0 para
formato normal)*/
//sinal de habilitação para o LCD
#define pulso_enable() _delay_us(1); set_bit(CONTR_LCD,E); _delay_us(1);clr_bit(CONTR_LCD,E); _delay_us(45)
//protótipo das funções
void cmd_LCD(unsigned char c, char cd);
void inic_LCD_4bits();
void escreve_LCD(char *c);
void escreve_LCD_Flash(const char *c);
void ident_num(unsigned int valor, unsigned char *disp);
#endif
