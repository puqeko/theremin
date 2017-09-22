// Timers.h
// 20-09-17
//
// Here be automagic...
// Some defines to help with setting the timer registers on an arduino uno

#include "Arduino.h"

#ifndef TIMERS_H
#define TIMERS_H

#define F_CPU 16000000 // 16 megahertz for the Arduino Uno
#define timer_pwm_freqency(prescaler) (F_CPU / (prescaler * 510.0))

// set and clear bits
#define BIT(bitnumber) (1 << bitnumber)
#define SET_BITS(reg, bits) (reg) |= (bits)
#define CLR_BITS(reg, bits) (reg) &= ~(bits)

// The uno has three timers
#define TIMER_0 0
#define TIMER_1 1
#define TIMER_2 2

// max timer values
#define TIMER0_SIZE 0x100  // 8-bit
#define TIMER1_SIZE 0x10000  // 16-bit
#define TIMER2_SIZE 0x100

// A helper that joins the parameters of name, timer, letter, and number togther
// for building register names from arguments.
#define JOIN(name, timer, letter, number) name ## timer ## letter ## number

// Overflow bit
#define timer_did_overflow(timer) (JOIN(TIFR, timer,,) & BIT(JOIN(TOV, timer,,)))
#define timer_clear_overflow(timer) CLR_BITS(JOIN(TIFR, timer,,), BIT(JOIN(TOV, timer,,)))


// Prescalar to divide clock speed
enum {
    TIMER_PRESCALER_NONE = 0x0,
    TIMER_PRESCALER_1 = 0x1,
    TIMER_PRESCALER_8 = 0x2,
    TIMER_PRESCALER_64 = 0x3,
    TIMER_PRESCALER_256 = 0x4,
    TIMER_PRESCALER_1024 = 0x5
};
#define PRESCALER_MASK 0x7
#define timer_set_prescaler(timer, prescaler) SET_BITS(JOIN(TCCR, timer, B,), prescaler);\
                                              CLR_BITS(JOIN(TCCR, timer, B,), ~prescaler & PRESCALER_MASK)
#define timer_enable_external_clock(timer) timer_set_prescaler(timer, 0x7)  // rising edge
#define timer_disable_external_clock(timer) timer_set_prescaler(timer, TIMER_PRESCALER_NONE)

// Compare and interupts
// There are two compare registiers, A and B. These can be set to values and told
// to trigger an interupt function when the counter value matches.
#define timer_set_compare_value(timer, compareLetter, value) JOIN(OCR, timer, compareLetter,) = (value)
#define timer_enable_interupt(timer, compareLetter) SET_BITS(JOIN(TIMSK, timer,,),\
            BIT(JOIN(OCIE, timer, compareLetter,)))
#define timer_disable_interupt(timer, compareLetter) CLR_BITS(JOIN(TIMSK, timer,,),\
            BIT(JOIN(OCIE, timer, compareLetter,)))
#define timer_enable_interupt_overflow(timer) SET_BITS(JOIN(TIMSK, timer,,),\
            BIT(JOIN(TOIE, timer,,)))

// Ouput compare mode has value from 0 to 3. It specifies how the compare values are to
// be used. The behaviour of the compare mode depends on the wave generation mode.
#define timer_set_compare_output_mode(timer, compareLetter, mode) \
        if (mode & 0x1) SET_BITS(JOIN(TCCR, timer, A,), BIT(JOIN(COM, timer, compareLetter, 0)));\
        else CLR_BITS(JOIN(TCCR, timer, A,), BIT(JOIN(COM, timer, compareLetter, 0)));\
        if (mode & 0x2) SET_BITS(JOIN(TCCR, timer, A,), BIT(JOIN(COM, timer, compareLetter, 1)));\
        else CLR_BITS(JOIN(TCCR, timer, A,), BIT(JOIN(COM, timer, compareLetter, 1)))

// Wave Generation
// Wave generation mode has a value from 0 to 7, (or 15 for timer 1 only). It specifies how the
// clock behaves. Read the manual for details.
#define WGM23 0
#define WGM03 0  // fix compile errors
#define timer_set_wavegen_mode(timer, mode) \
        if (mode & 0x1) SET_BITS(JOIN(TCCR, timer, A,), BIT(JOIN(WGM, timer, 0,)));\
        else CLR_BITS(JOIN(TCCR, timer, A,), BIT(JOIN(WGM, timer, 0,)));\
        if (mode & 0x2) SET_BITS(JOIN(TCCR, timer, A,), BIT(JOIN(WGM, timer, 1,)));\
        else CLR_BITS(JOIN(TCCR, timer, A,), BIT(JOIN(WGM, timer, 1,)));\
        if (mode & 0x4) SET_BITS(JOIN(TCCR, timer, B,), BIT(JOIN(WGM, timer, 2,)));\
        else CLR_BITS(JOIN(TCCR, timer, B,), BIT(JOIN(WGM, timer, 2,)));\
        if (timer == TIMER_1 && mode & 0x8) SET_BITS(JOIN(TCCR, timer, B,), BIT(JOIN(WGM, timer, 3,)));\
        else if (timer == TIMER_1) CLR_BITS(JOIN(TCCR, timer, B,), BIT(JOIN(WGM, timer, 3,)))


// Initalise registers to zero.
extern void clear_registers();

// REGISTER LAYOUT
//                    TIMER 0 (System Clock, 8 Bits)
// ==================================================================

// Control Register 0A (TCCR0A)
//     Compare Match Output A & B              Wave Generation Mode
// |==================================================================|
// | COM0A1 | COM0A0 | COM0B1 | COM0B0 | ----- | ---- | WGM01 | WGM00 |
// |==================================================================|
// #define REGISTER_TIMER0_CTRL_A TCCR0A
//
// Control Register 0B (TCCR0B)
//     Force Output Compare     Wave Generation Mode, Clock Setting
// |==================================================================|
// | FOC0A  | FOC0B  | ------ | ------ | WGM02 | CS02 | CS01  | CS00  |
// |==================================================================|
// #define REGISTER_TIMER0_CTRL_B TCCR0B
// Interupt Mask Register (TIMSK0)
// Ouput Compare Interupt Enable (A & B), Overflow Interupt Enable
// |==================================================================|
// | ------ | ------ | ---- | -----  | ----- | OCIEB | OCIEA  | TOIE  |
// |==================================================================|
// #define REGISTER_TIMER0_INTERUPT_MASK TIMSK0
// Interupt Flag Register (TIFR0)
// Set when a matching event in the Interup Mask Register occures
// |=================================================================|
// | ------ | ------ | ----- | -----  | ----- | OCFB  | OCFA  | TOV  |
// |=================================================================|


//                      TIMER 1 (Servos, 16 bits)
// ==================================================================

// Control Register 1A (TCCR1A)
//     Compare Match Output A & B              Wave Generation Mode
// |==================================================================|
// | COM1A1 | COM1A0 | COM1B1 | COM1B0 | ----- | ---- | WGM11 | WGM10 |
// |==================================================================|
// #define REGISTER_TIMER1_CTRL_A TCCR1A
// The Compare Match bits control the values of OC1A (output pin, port B bit 1)
//
// Control Register 1B (TCCR1B)
//                             Wave Generation Mode,  Clock Setting
// |==================================================================|
// | ------ | ------ | ------ | WGM13  | WGM12 | CS12 | CS11  | CS10  |
// |==================================================================|
// #define REGISTER_TIMER1_CTRL_B TCCR1B
// Output Compare Registers A & B (OCR1A & OCR1B)
// Generate an interupt or a a waveform output on a OC1X pin.
//
// Interupt Mask Register (TIMSK1)
// Input Capture Interupt, Ouput Compare Interupt Enable (A & B), Overfow Interupt Enable
// |==================================================================|
// | ------ | ------ | ICIE | -----  | ----- | OCIEB | OCIEA  | TOIE  |
// |==================================================================|
// #define REGISTER_TIMER1_INTERUPT_MASK TIMSK1
//
// Interupt Flag Register (TIFR1)
// Set when a matching event in the Interup Mask Register occures
// |================================================================|
// | ------ | ------ | ICF  | -----  | ----- | OCFB  | OCFA  | TOV  |
// |================================================================|



//                     TIMER 2 (Tone library, 8 bit)
// ==================================================================

// Control Register 2A (TCCR2A)
//     Compare Match Output A & B              Wave Generation Mode
// |==================================================================|
// | COM2A1 | COM2A0 | COM2B1 | COM2B0 | ----- | ---- | WGM21 | WGM20 |
// |==================================================================|
// #define REGISTER_TIMER2_CTRL_A TCCR2A
// Control Register 2B (TCCR2B)
//     Force Output Compare     Wave Generation Mode, Clock Setting
// |==================================================================|
// | FOC2A  | FOC2B  | ------ | ------ | WGM22 | CS22 | CS21  | CS20  |
// |==================================================================|
// #define REGISTER_TIMER2_CTRL_B TCCR2B

// // Counter values
#define REGISTER_TIMER0_COUNTER_VALUE TCNT0  // 8 bit
#define REGISTER_TIMER1_COUNTER_VALUE TCNT1  // 16 bit
#define REGISTER_TIMER2_COUNTER_VALUE TCNT2  // 8 bit

// // Register to compare counter values with
// #define REGISTER_TIMER0_OUTPUT_COMPARE OCR0A  // pin OC0A
// #define REGISTER_TIMER0_OUTPUT_COMPARE OCR0B  // pin OC0B
#endif