// Timers.c
// 20-09-17
// Support funtion for using builtin timers.

#include "timers.h"

// reset timer's 1 and 2
// leave timer 0 alone
void clear_registers()
{
    TCCR1A = TCCR1B = TCCR2A = TCCR2B = 0;  // reset control registers
    TCNT1 = TCNT2 = 0;  // counter value
    TIFR1 = TIFR2 = 0;  // flag/event register
}