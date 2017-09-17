#define SET_BITS(reg, bits) (reg) |= (bits)
#define CLR_BITS(reg, bits) (reg) &= ~(bits)


//                              TIMER 1
// ==================================================================
#define REGISTER_T1_CTRL_A TCCR1A
#define REGISTER_T1_CTRL_B TCCR1B
//
// Control Register 1A (TCCR1A & TCCR1B)
//     Compare Match Output A & B              Wave Generation Mode
// |==================================================================|
// | COM1A1 | COM1A0 | COM1B1 | COM1B0 | ----- | ---- | WGM11 | WGM10 |
// |==================================================================|
//
// Control Register 2
//                             Wave Generation Mode,  Clock Setting
// |==================================================================|
// | ------ | ------ | ------ | WGM13  | WGM12 | CS12 | CS11  | CS10  |
// |==================================================================|
//
// Output Compare Registers A & B (OCR1A & OCR1B)
// Generate an interupt or a a waveform output on a OC1X pin.
//
// Interupt Mask Register (TIMSK1)
// Input Capture Interupt, Ouput Compare Interupt Enable (A & B), Overfow Interupt Enable
// |==================================================================|
// | ------ | ------ | ICIE | -----  | ----- | OCIEB | OCIEA  | TOIE  |
// |==================================================================|
//
// Interupt Flag Register (TIFR1)
// Set when a matching event in the Interup Mask Register occures
// |================================================================|
// | ------ | ------ | ICF  | -----  | ----- | OCFB  | OCFA  | TOV  |
// |================================================================|


//                              TIMER 2
// ==================================================================
#define REGISTER_T2_CTRL_A TCCR2A
#define REGISTER_T2_CTRL_B TCCR2B

void initFreqy();
double getFreqy();