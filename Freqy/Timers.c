#include "Timers.h"

void timer_set_prescaler(TIMER_ID id, uint8_t value);
void timer_set_mode(TIMER_ID id, uint8_t mode);
void timer_set_compare_mode(TIMER_ID id, uint_8 mode);
void timer_set_compare_value(TIMER_ID id, uint16_t compareValue);

#define VALUE(x) x
#define _timer_did_overflow(timer) (TIFR ## timer & TOV ## timer)
#define _timer_clear_overflow(timer) (TIFR ## timer &= ~TOV ## timer)
#define timer_did_overflow(timer) timer_did_overflow(timer)
#define timer_clear_overflow(timer) timer_clear_overflow(timer)

void timer_set_interupt(TIMER_ID id, TIMER_INTERUPT interupt)
{

}

bool timer_did_overflow(TIMER_ID id)
{
    switch (id) {
        case TIMER_0:
            return TIFR0 & TOV0;
        case TIMER_1:
            return TIFR1 & TOV1;
        default:
            return TIFR2 & TOV2;
    }
}

void timer_clear_overflow(TIMER_ID id)
{
    switch (id) {
        case TIMER_0:
            TIFR0 &= ~TOV0;
            break;
        case TIMER_1:
            TIFR1 &= ~TOV1;
            break;
        default:
            TIFR2 &= ~TOV2;
    }
}