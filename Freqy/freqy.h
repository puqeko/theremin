// Freqy.h - Freqency measurment on Arduino Uno.
// ENEL200 Design Project, building a theremin.
// 17-09-17, Group 13

#include "Arduino.h"

#ifndef FREQ_H
#define FREQ_H

void init_freqy();
double get_input_freqy();
void set_output_freqy(float newFreqency);

#endif