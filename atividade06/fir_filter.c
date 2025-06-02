// fir_filter.c
#include "fir_filter.h"

#define FILTER_TAPS 16
#define GAIN_SHIFT  7  // Ajuste de ganho (2^7 = 128)

int16_t fir_lowpass(int16_t input) {
  static int16_t delay_line[FILTER_TAPS] = {0};
  int32_t acc = 0;
  
  // Desliza o histórico
  for (int i = FILTER_TAPS-1; i > 0; i--) {
    delay_line[i] = delay_line[i-1];
  }
  delay_line[0] = input;  // Agora usa 16 bits diretamente!

  // Convolução (multiplicação 16x16 bits)
  for (int i = 0; i < FILTER_TAPS; i++) {
    acc += (int32_t)delay_line[i] * fir_coeffs[i];
  }

  return (int16_t)(acc >> GAIN_SHIFT);  // Normalização
}