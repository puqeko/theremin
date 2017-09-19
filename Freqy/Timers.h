#include <stdbool.h>

typedef timer16_t volitile uint16_t

enum TIMER_INTERUPT {
    TIMER_INTERUPT_ENABLE_COMPARE_A = 0,
    TIMER_INTERUPT_ENABLE_COMPARE_B,
    TIMER_INTERUPT_ENABLE_OVERFLOW
};

void timer_set_prescaler(TIMER_ID id, uint8_t value);
void timer_set_mode(TIMER_ID id, uint8_t mode);
void timer_set_compare_mode(TIMER_ID id, uint_8 mode);
void timer_set_compare_value(TIMER_ID id, uint16_t compareValue);
void timer_set_interupt(TIMER_ID id, TIMER_INTERUPT interupt);
void timer_clear_overflow(TIMER_ID id);

bool timer_did_overflow(TIMER_ID id);