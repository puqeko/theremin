#define SET_BITS(reg, bits) (reg) |= (bits)
#define CLR_BITS(reg, bits) (reg) &= ~(bits)

//                    TIMER 0 (System Clock, 8 Bits)
// ==================================================================
// Control Register 0A (TCCR0A)
//     Compare Match Output A & B              Wave Generation Mode
// |==================================================================|
// | COM0A1 | COM0A0 | COM0B1 | COM0B0 | ----- | ---- | WGM01 | WGM00 |
// |==================================================================|
#define REGISTER_T1_CTRL_A TCCR0A
//
// Control Register 0B (TCCR0B)
//     Force Output Compare     Wave Generation Mode, Clock Setting
// |==================================================================|
// | FOC0A  | FOC0B  | ------ | ------ | WGM02 | CS02 | CS01  | CS00  |
// |==================================================================|
#define REGISTER_T1_CTRL_B TCCR0B


//                      TIMER 1 (Servos, 16 bits)
// ==================================================================
// Control Register 1A (TCCR1A)
//     Compare Match Output A & B              Wave Generation Mode
// |==================================================================|
// | COM1A1 | COM1A0 | COM1B1 | COM1B0 | ----- | ---- | WGM11 | WGM10 |
// |==================================================================|
#define REGISTER_T1_CTRL_A TCCR1A
// The Compare Match bits control the values of OC1A (output pin, port B bit 1)
//
// Control Register 1B (TCCR1B)
//                             Wave Generation Mode,  Clock Setting
// |==================================================================|
// | ------ | ------ | ------ | WGM13  | WGM12 | CS12 | CS11  | CS10  |
// |==================================================================|
#define REGISTER_T1_CTRL_B TCCR1B


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


//                     TIMER 2 (Tone library, 8 bit)
// ==================================================================
// Control Register 2A (TCCR2A)
//     Compare Match Output A & B              Wave Generation Mode
// |==================================================================|
// | COM2A1 | COM2A0 | COM2B1 | COM2B0 | ----- | ---- | WGM21 | WGM20 |
// |==================================================================|
#define REGISTER_T2_CTRL_A TCCR2A
// Control Register 2B (TCCR2B)
//     Force Output Compare     Wave Generation Mode, Clock Setting
// |==================================================================|
// | FOC2A  | FOC2B  | ------ | ------ | WGM22 | CS22 | CS21  | CS20  |
// |==================================================================|
#define REGISTER_T2_CTRL_B TCCR2B

void initFreqy();
double getFreqy();