// Freqy.c - Freqency measurment on Arduino Uno.
// ENEL200 Design Project, building a theremin.
// 17-09-17, Group 13

#include "freqy.h"

static const float pwmFreqency = get_pwm_freqency(TIMER_PRESCALER_1);
static float timestep = 0.0;
static float outputFreqency = 0.0;
static float phaseAccumulator = 0.0;

static float calc_timestep(float signalFreqency)
{
    // rearrange from signalFreqency = (pwmFreqency / 256) * timestep
    return 256.0 * signalFreqency / pwmFreqency;
}

// Setup registers and initalise values. Called once in setup.
void init_freqy()
{
    clear_registers();  // clear timer registers
    timer_enable_interupt_overflow(TIMER_2);
    timer_set_prescaler(TIMER_2, TIMER_PRESCALER_1);

    // Use phase correct PWM with TOP = 0xFF, compare update TOP, overflow when BOTTOM
    timer_set_wavegen_mode(TIMER_2, 0x1);

    // Clear OC2A (output compare pin) on Compare Match when up-counting. Set ouput
    // compare pin A on Compare Match when down-counting.
    // Change of signal (low->high, or high->low) occures when compare value met.
    timer_set_compare_output_mode(TIMER_2, A, 0x2);
}

// when counter values is zero (BOTTOM).
ISR(TIMER2_OVF_vect) {

}

// Return the measured freqency.
double get_input_freqy()
{

}

// Set output freqency
void set_output_freqy(float newFreqency)
{
    outputFreqency = newFreqency;
    timestep = calc_timestep(newFreqency);
}