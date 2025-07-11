
#ifndef FIR_FILTER_H
#define FIR_FILTER_H

#include <stdint.h>


int16_t fir_lowpass(int16_t input, volatile int16_t cf[]);

#endif