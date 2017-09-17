// Freqy - freqency measurment on Arduino Uno.
// ENEL200 Design Project, building a theremin.
// 17-09-17, Group 13
//
// Use one timer as a reference, then check the count of the clocked timer every sampleTimeMSecs

long measuredFreqency = 0;
int sampleTimeMSecs = 100;  // time between sampleTimeMSecs

// Setup registers and initalise values. Called once in setup.
void initFreqy()
{
    // Control Register 1
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

    // Pin T1, rising edge triggers counter from external clock
    SET_BITS(REGISTER_T1_CTRL_B, CS10 | CS11 | CS12);

    SET_BITS(REGISTER_T2_CTRL_B, CS21 | CS22);
    CLR_BITS(REGISTER_T2_CTRL_B, CS20);
}

// Return the measured freqency.
double getFreqy()
{

}