#include "Timers.h"

void clear_registers()
{
    TCCR0A = TCCR0B = TCCR1A = TCCR1B = TCCR2A = TCCR2B = 0;  // reset control registers
    TCNT0 = TCNT1 = TCNT2 = 0;  // counter value
    TIFR0 = TIFR1 = TIFR2 = 0;  // flag/event register
}