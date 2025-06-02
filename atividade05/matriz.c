#include "matriz.h"

#define CS_PIN     PB2    // Chip Select pin on PORTB

#define CS_LOW()   PORTB &= ~(1 << CS_PIN)
#define CS_HIGH()  PORTB |=  (1 << CS_PIN)

void spi_init(void) {
    // Set MOSI (PB3), SCK (PB5), and CS (PB2) as output
    DDRB |= (1 << PB3) | (1 << PB5) | (1 << CS_PIN);

    // Enable SPI, Master mode, fosc/16
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void spi_send(uint8_t data) {
    SPDR = data;                       // Load data into SPI data register
    while (!(SPSR & (1 << SPIF)));     // Wait for transmission complete
}

void max7219_send(uint8_t address, uint8_t data) {
    CS_LOW();
    spi_send(address);
    spi_send(data);
    CS_HIGH();
}

void max7219_init(void) {
    max7219_send(0x0C, 0x01); // Normal operation
    max7219_send(0x0F, 0x00); // Display test off
    max7219_send(0x0B, 0x07); // Scan limit = 8 digits
    max7219_send(0x0A, 0x08); // Intensity = 8/15
    max7219_send(0x09, 0x00); // Decode mode = none (matrix mode)
}

void max7219_clear(void) {
    for (uint8_t i = 1; i <= 8; i++) {
        max7219_send(i, 0x00);
    }
}

void max7219_draw_dot(int x, int y){
    if(x>7 || y>7) return;

    max7219_clear();
    max7219_send(y+1, (1<<x));
}
void convertM(int x[], int y[]){
    uint8_t matrixaux [8] = {0,0,0,0,0,0,0,0};
    for(int i=0; i<8; i++){
        if(y[i] == 0 && x[i]!=0){
            matrixaux[0] |= (1<<x[i]);
        }
         if(y[i] == 1){
            matrixaux[1] |= (1<<x[i]);
        }
         if(y[i] == 2){
            matrixaux[2] |= (1<<x[i]);
        }
         if(y[i] == 3){
            matrixaux[3] |= (1<<x[i]);
        }
          if(y[i] == 4){
            matrixaux[4] |= (1<<x[i]);
        }
         if(y[i] == 5){
            matrixaux[5] |= (1<<x[i]);
        }
         if(y[i] == 6){
            matrixaux[6] |= (1<<x[i]);
        }
         if(y[i] == 7){
            matrixaux[7] |= (1<<x[i]);
        }
        
    }
    max7219_clear();
    for(int i=0; i<7; i++){
        max7219_send(i+1, matrixaux[i]);
    }
}
void max7219_draw_dot_trail(int x[], int y[], int sizeoftrail){
    max7219_clear();
    for(int i =0; i<sizeoftrail; i++){
        max7219_send(y[i]+1, (1<<x[i]));
    }
}