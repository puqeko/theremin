// Freqy.h - Freqency measurment on Arduino Uno.
// ENEL200 Design Project, building a theremin.
// 17-09-17, Group 13

#include "Timers.h"

#define F_CPU 16000000 // 16 megahertz
#define get_pwm_freqency(prescaler) (F_CPU / (prescaler * 510.0))

void init_freqy();
void set_input_freqy(float newFreqency);
double set_output_freqy();