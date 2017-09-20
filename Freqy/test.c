#define SET_BITS(reg, bits) (reg) |= (bits)
#define CLR_BITS(reg, bits) (reg) &= ~(bits)

#define TIMER_0 0
#define TIMER_1 1
#define TIMER_2 2

#define JOIN(reg, t, l, n) reg ## t ## l ## n
#define timer_did_overflow(timer) (JOIN(TIFR, timer,,) & JOIN(TOV, timer,,))
#define timer_clear_overflow(timer) CLR_BITS(JOIN(TIFR, timer,,), JOIN(TOV, timer,,))
#define timer_enable_overflow(timer) SET_BITS(JOIN(TIMSK, timer,,), JOIN(TOIE, timer,,))

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

// There are two compare registiers, A and B. These can be set to values and told
// to trigger an interupt function when the counter value matches.
#define timer_set_compare_value(timer, compareLetter, value) JOIN(OCR, timer, compareLetter,) = (value)
#define timer_enable_interupt(timer, compareLetter) SET_BITS(JOIN(TIMSK, timer,,),\
            JOIN(OCIE, timer, compareLetter,))
#define timer_disable_interupt(timer, compareLetter) CLR_BITS(JOIN(TIMSK, timer,,),\
            JOIN(OCIE, timer, compareLetter,))

// Ouput compare mode has value from 0 to 3. It specifies how the compare values are to
// be used. The behaviour of the compare mode depends on the wave generation mode.
#define timer_set_compare_output_mode(timer, compareLetter, mode) \
        if (mode & 0x1) SET_BITS(JOIN(TCCR, timer, A,), JOIN(COM, timer, compareLetter, 0));\
        else CLR_BITS(JOIN(TCCR, timer, A,), JOIN(COM, timer, compareLetter, 0));\
        if (mode & 0x2) SET_BITS(JOIN(TCCR, timer, A,), JOIN(COM, timer, compareLetter, 1));\
        else CLR_BITS(JOIN(TCCR, timer, A,), JOIN(COM, timer, compareLetter, 1))

// Wave generation mode has value from 0 to 7, (or 15 for timer 1 only). It specifies how the
// clock behaves. Read the manual for details.
#define timer_set_wavegen_mode(timer, mode) \
        if (mode & 0x1) SET_BITS(JOIN(TCCR, timer, A,), JOIN(WGM, timer, 0,));\
        else CLR_BITS(JOIN(TCCR, timer, A,), JOIN(WGM, timer, 0,));\
        if (mode & 0x2) SET_BITS(JOIN(TCCR, timer, A,), JOIN(WGM, timer, 1,));\
        else CLR_BITS(JOIN(TCCR, timer, A,), JOIN(WGM, timer, 1,));\
        if (mode & 0x4) SET_BITS(JOIN(TCCR, timer, B,), JOIN(WGM, timer, 2,));\
        else CLR_BITS(JOIN(TCCR, timer, B,), JOIN(WGM, timer, 2,));\
        if (timer == TIMER_1 && mode & 0x8) SET_BITS(JOIN(TCCR, timer, B,), JOIN(WGM, timer, 3,));\
        else if (timer == TIMER_1) CLR_BITS(JOIN(TCCR, timer, B,), JOIN(WGM, timer, 3,))


// Initalise registers to zero.
void clear_registers()
{
    TCCR0A = TCCR0B = TCCR1A = TCCR1B = TCCR2A = TCCR2B = 0;  // reset control registers
    TCNT0 = TCNT1 = TCNT2 = 0;  // counter value
    TIFR0 = TIFR1 = TIFR2 = 0;  // flag/event register
}

timer_set_prescaler(TIMER_2, TIMER_PRESCALE_265);
timer_enable_external_clock(TIMER_1);
timer_set_compare_value(TIMER_1, A, 200);
timer_enable_interupt(TIMER_0, B);
timer_set_compare_output_mode(TIMER_1, A, 1);
timer_set_compare_output_mode(TIMER_1, B, 3);
timer_set_wavegen_mode(TIMER_1, 0x10);