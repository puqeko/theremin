// Freqy.h - Freqency measurment on Arduino Uno.
// ENEL200 Design Project, building a theremin.
// 17-09-17, Group 13
//
// Use one timer as a reference, then check the count of the clocked timer every sampleTimeMSecs

#define SET_BITS(reg, bits) (reg) |= (bits)
#define CLR_BITS(reg, bits) (reg) &= ~(bits)

void init_freqy();
void set_freqy();
double get_freqy();