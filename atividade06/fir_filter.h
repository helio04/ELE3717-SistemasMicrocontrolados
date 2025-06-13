//prototipo
// fir_filter.h
#ifndef FIR_FILTER_H
#define FIR_FILTER_H

#include <stdint.h>

// Coeficientes em 16 bits (valores entre -32768 e 32767)
const int16_t fir_coeffs[16] = {
  -1200, -800, 400, 1800, 3000, 3800, 3800, 3000,
  1800, 400, -800, -1200, -1600, -1600, -1200, -800
};

// Protótipo
int16_t fir_lowpass(int16_t input);
int16_t fir_lowpass(int16_t input, int16_t cf[]);
#endif