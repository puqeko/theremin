// Freqy.c - Freqency measurment on Arduino Uno.
// ENEL200 Design Project, building a theremin.
// 17-09-17, Group 13

#include "freqy.h"

// Use one timer as a reference, then check the count of the clocked timer every sampleTimeMSecs
long measuredFreqency = 0;
int sampleTimeMSecs = 100;  // time between sampleTimeMSecs

// Setup registers and initalise values. Called once in setup.
void init_freqy()
{
    clear_registers();  // clear timer registers
    timer_enable_external_clock(TIMER_1); // pin T1 (#5) as sqaure wave input
}

// Return the measured freqency.
double get_freqy()
{

}

// Set output freqency
void set_freqy()
{

}