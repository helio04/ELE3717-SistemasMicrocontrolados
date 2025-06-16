// fir_filter.c
#include "fir_filter.h"

#define FILTER_TAPS 16
#define GAIN_SHIFT  12  // Ajuste de ganho (2^7 = 128)
/*
int16_t fir_lowpass2(int16_t input) {
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
}*/
int16_t fir_lowpass(int16_t input, volatile int16_t cf[]){
  static int16_t delay_line_t[FILTER_TAPS] = {0};
  int32_t acc = 0;
  
  for (int i = FILTER_TAPS-1; i > 0; i--) {
    delay_line_t[i] = delay_line_t[i-1];
  }
  delay_line_t[0] = input;  // Agora usa 16 bits diretamente!

  // Convolução (multiplicação 16x16 bits)
  for (int i = 0; i < FILTER_TAPS; i++) {
    acc += (int32_t)delay_line_t[i] * cf[i];
  }

  return (int16_t)(acc >> GAIN_SHIFT);
}
float fir_lowpass_float(float input, const float cf[]) {
    static float delay_line_t[FILTER_TAPS] = {0.0f};  // Floating-point delay line
    float acc = 0.0f;                                 // Floating-point accumulator

    // Shift delay line (same logic)
    for (int i = FILTER_TAPS - 1; i > 0; i--) {
        delay_line_t[i] = delay_line_t[i - 1];
    }
    delay_line_t[0] = input;  // Store current input sample

    // Convolution with floating-point coefficients
    for (int i = 0; i < FILTER_TAPS; i++) {
        acc += delay_line_t[i] * cf[i];  // No casting required
    }

    return acc;  // Already in floating-point form
}
