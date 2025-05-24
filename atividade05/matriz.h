#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define CS_PIN     PB2    // Chip Select pin on PORTB

#define CS_LOW()   PORTB &= ~(1 << CS_PIN)
#define CS_HIGH()  PORTB |=  (1 << CS_PIN)

void spi_init(void);
void spi_send(uint8_t data);
void max7219_send(uint8_t address, uint8_t data);
void max7219_init(void);
void max7219_clear(void);
void max7219_draw_dot(int x, int y);
void max7219_draw_dot_trail(int x[], int y[], int size);