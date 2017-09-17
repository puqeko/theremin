#define SET_BITS(reg, bits) (reg) |= (bits)
#define CLR_BITS(reg, bits) (reg) &= ~(bits)

#define REGISTER_T1_CTRL_A TCCR1A
#define REGISTER_T1_CTRL_B TCCR1B
#define REGISTER_T2_CTRL_A TCCR2A
#define REGISTER_T2_CTRL_B TCCR2B

void initFreqy();
double getFreqy();