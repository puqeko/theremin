// filter.h - Frequency measurment on Arduino Uno.
// ENEL200 Design Project, building a theremin.
// 25-09-17, Group 13

#include "Arduino.h"

#ifndef FILTER_H
#define FILTER_H

// Set the inital value to high instead of low.
void filter_init_with(double initValue);

// Return the result of the filter being applied to nextDistance.
double filter_apply(float nextDistance);

#endif