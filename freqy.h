// freqy.h - Freqency measurment on Arduino Uno.
// ENEL200 Design Project, building a theremin.
// 17-09-17, Group 13

#include "Arduino.h"

#ifndef FREQ_H
#define FREQ_H

void frequency_init(void);
void frequency_set(float newFreqency);

uint32_t frequency_read(float captureTime);

#endif