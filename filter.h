// filter.h - Freqency measurment on Arduino Uno.
// ENEL200 Design Project, building a theremin.
// 25-09-17, Group 13

#include "Arduino.h"

#ifndef FILTER_H
#define FILTER_H

// Set the inital value to high instead of low
void init_filter_with(double initValue);

// Return the result of the filter being applied to nextDistance.
double apply_filter(float nextDistance);

#endif