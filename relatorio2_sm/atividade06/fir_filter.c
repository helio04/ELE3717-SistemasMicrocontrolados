// fir_filter.c
#include "fir_filter.h"

#define FILTER_TAPS 16
#define GAIN_SHIFT  12  
int16_t fir_lowpass(int16_t input, volatile int16_t cf[]){
  static int16_t delay_line_t[FILTER_TAPS] = {0};
  int32_t acc = 0;
  
  for (int i = FILTER_TAPS-1; i > 0; i--) {
    delay_line_t[i] = delay_line_t[i-1];
  }
  delay_line_t[0] = input;  
  for (int i = 0; i < FILTER_TAPS; i++) {
    acc += (int32_t)delay_line_t[i] * cf[i];
  }

  return (int16_t)(acc >> GAIN_SHIFT);
}
