#define JOIN(reg, t) reg ## t

#define timer_did_overflow(timer) (JOIN(TIFR, timer) & JOIN(TOV, timer))
#define timer_clear_overflow(timer) (JOIN(TIFR, timer) &= ~JOIN(TOV, timer))

#define TIMER_0 0
#define TIMER_1 1
#define TIMER_2 2

int main (void)
{
    timer_clear_overflow(TIMER_0);
}