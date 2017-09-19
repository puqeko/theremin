// Freqy.h - Freqency measurment on Arduino Uno.
// ENEL200 Design Project, building a theremin.
// 17-09-17, Group 13
//
// Use one timer as a reference, then check the count of the clocked timer every sampleTimeMSecs

#define SET_BITS(reg, bits) (reg) |= (bits)
#define CLR_BITS(reg, bits) (reg) &= ~(bits)



//                    TIMER 0 (System Clock, 8 Bits)
// ==================================================================

// Control Register 0A (TCCR0A)
//     Compare Match Output A & B              Wave Generation Mode
// |==================================================================|
// | COM0A1 | COM0A0 | COM0B1 | COM0B0 | ----- | ---- | WGM01 | WGM00 |
// |==================================================================|
#define REGISTER_Timer0_CTRL_A TCCR0A
//
// Control Register 0B (TCCR0B)
//     Force Output Compare     Wave Generation Mode, Clock Setting
// |==================================================================|
// | FOC0A  | FOC0B  | ------ | ------ | WGM02 | CS02 | CS01  | CS00  |
// |==================================================================|
#define REGISTER_Timer0_CTRL_B TCCR0B
// Interupt Mask Register (TIMSK0)
// Ouput Compare Interupt Enable (A & B), Overflow Interupt Enable
// |==================================================================|
// | ------ | ------ | ---- | -----  | ----- | OCIEB | OCIEA  | TOIE  |
// |==================================================================|
#define REGISTER_Timer0_INTERUPT_MASK TIMSK0
// Interupt Flag Register (TIFR0)
// Set when a matching event in the Interup Mask Register occures
// |=================================================================|
// | ------ | ------ | ----- | -----  | ----- | OCFB  | OCFA  | TOV  |
// |=================================================================|


//                      TIMER 1 (Servos, 16 bits)
// ==================================================================

// Control Register 1A (TCCR1A)
//     Compare Match Output A & B              Wave Generation Mode
// |==================================================================|
// | COM1A1 | COM1A0 | COM1B1 | COM1B0 | ----- | ---- | WGM11 | WGM10 |
// |==================================================================|
#define REGISTER_Timer1_CTRL_A TCCR1A
// The Compare Match bits control the values of OC1A (output pin, port B bit 1)
//
// Control Register 1B (TCCR1B)
//                             Wave Generation Mode,  Clock Setting
// |==================================================================|
// | ------ | ------ | ------ | WGM13  | WGM12 | CS12 | CS11  | CS10  |
// |==================================================================|
#define REGISTER_Timer1_CTRL_B TCCR1B
// Output Compare Registers A & B (OCR1A & OCR1B)
// Generate an interupt or a a waveform output on a OC1X pin.
//
// Interupt Mask Register (TIMSK1)
// Input Capture Interupt, Ouput Compare Interupt Enable (A & B), Overfow Interupt Enable
// |==================================================================|
// | ------ | ------ | ICIE | -----  | ----- | OCIEB | OCIEA  | TOIE  |
// |==================================================================|
#define REGISTER_Timer1_INTERUPT_MASK TIMSK1
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
#define REGISTER_Timer2_CTRL_A TCCR2A
// Control Register 2B (TCCR2B)
//     Force Output Compare     Wave Generation Mode, Clock Setting
// |==================================================================|
// | FOC2A  | FOC2B  | ------ | ------ | WGM22 | CS22 | CS21  | CS20  |
// |==================================================================|
#define REGISTER_Timer2_CTRL_B TCCR2B

// Counter values
#define REGISTER_Timer0_COUNTER_VALUE TCNT0  // 8 bit
#define REGISTER_Timer1_COUNTER_VALUE TCNT1  // 16 bit
#define REGISTER_Timer2_COUNTER_VALUE TCNT2  // 8 bit

// Register to compare counter values with
#define REGISTER_Timer0_OUTPUT_COMPARE OCR0A  // pin OC0A
#define REGISTER_Timer0_OUTPUT_COMPARE OCR0B  // pin OC0B

void initFreqy();
void setFreqy();
double getFreqy();