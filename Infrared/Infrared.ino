/*
 * ENEL200 Design Project (Theremin)
 * Ultrasonic controlled PWM sinewave generator.
 * 
 * Adapted from:
 * KHM 2009 /  Martin Nawrath
 * Kunsthochschule fuer Medien Koeln
 * Academy of Media Arts Cologne
 * 
 * Editied 26/08/2017
 * 
 * Plug:
 * -> Vcc into 5v
 * -> Gnd into grd
 * -> Echo into Pin 7
 * -> Trig into Pin 8
 */

 // Research
// http://www.analog.com/en/analog-dialogue/articles/all-about-direct-digital-synthesis.html
// https://see.stanford.edu/materials/lsoftaee261/book-fall-07.pdf
// http://hyperphysics.phy-astr.gsu.edu/hbase/Music/flutew.html#c3
// http://interface.khm.de/index.php/lab/interfaces-advanced/theremin-as-a-capacitive-sensing-device/
// https://www.dataq.com/data-acquisition/general-education-tutorials/fft-fast-fourier-transform-waveform-analysis.html
// Datasheet: http://www.atmel.com/Images/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf

#include <avr/pgmspace.h>
#include <math.h>

// 256 sine values, one sine period, stored in flash memory (save dynamic memory space)
const PROGMEM  unsigned char sine256[]  = {
  127,130,133,136,139,143,146,149,152,155,158,161,164,167,170,173,176,178,181,184,187,190,192,
  195,198,200,203,205,208,210,212,215,217,219,221,223,225,227,229,231,233,234,236,238,239,240,
  242,243,244,245,247,248,249,249,250,251,252,252,253,253,253,254,254,254,254,254,254,254,253,
  253,253,252,252,251,250,249,249,248,247,245,244,243,242,240,239,238,236,234,233,231,229,227,
  225,223,221,219,217,215,212,210,208,205,203,200,198,195,192,190,187,184,181,178,176,173,170,
  167,164,161,158,155,152,149,146,143,139,136,133,130,127,124,121,118,115,111,108,105,102,99,
  96,93,90,87,84,81,78,76,73,70,67,64,62,59,56,54,51,49,46,44,42,39,37,35,33,31,29,27,25,23,
  21,20,18,16,15,14,12,11,10,9,7,6,5,5,4,3,2,2,1,1,1,0,0,0,0,0,0,0,1,1,1,2,2,3,4,5,5,6,7,9,
  10,11,12,14,15,16,18,20,21,23,25,27,29,31,33,35,37,39,42,44,46,49,51,54,56,59,62,64,67,
  70,73,76,78,81,84,87,90,93,96,99,102,105,108,111,115,118,121,124
};
#define clearBit(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define setBit(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

double frequency = 0.0;
// const double refclk=31372.549;  // =16MHz / 510
const double refclk = 31376.6;      // measured

// variables used inside interrupt service declared as voilatile
volatile byte icnt;              // var inside interrupt
volatile uint32_t phaccu;   // phase accumulator
volatile uint32_t tword_m;  // dds tuning word m

#define inputPin A0   // Echo Pin
#define PWMPin 11   // PWM Output
#define KERNAL_SIZE 23

#define MAX_RELYABLE_DISTANCE 120.0  // stop extreme readings from ultrasonics
#define MAX_ULTRASONIC_DISTANCE 100.0  // in cm
#define MAX_OUTPUT_FREQENCY 1500.0

double distanceBuffer[KERNAL_SIZE] = {0.0};
double filterKernal[KERNAL_SIZE] = {2.842170943040401e-14, 1.2789769243681803e-12, 2.8137492336099967e-11, 4.0330405681743287e-10, 4.234692596583045e-09, 3.472447929198097e-08, 2.3149652861320646e-07, 1.2897663737021503e-06, 6.126390275085214e-06, 2.518627113090588e-05, 9.067057607126117e-05, 0.00028849728749946735, 0.0008174089812484908, 0.002074961260092323, 0.004742768594496738, 0.009801721761959925, 0.01837822830367486, 0.031351095341562996, 0.048768370531320215, 0.06930242128134978, 0.09009314766575471, 0.10725374722113656, 0.11700408787760352};
int nextPos = 3;

void setup()
{
  Serial.begin(57600);        // connect to the serial port
  Serial.println("DDS Test");

  pinMode(PWMPin, OUTPUT);     // pin11= PWM  output / frequency output

  Setup_timer2();

  // disable interrupts to avoid timing distortion
  //clearBit (TIMSK0,TOIE0);              // disable Timer0 !!! delay() is now not available
  setBit (TIMSK2,TOIE2);              // enable Timer2 Interrupt

  tword_m = pow(2,32) * frequency / refclk;  // calulate DDS new tuning word

  // so that there is no transient sound on startup, initalise to max value
  for (int i = 0; i < KERNAL_SIZE; distanceBuffer[i++] = MAX_RELYABLE_DISTANCE);
}

// true modulus, result is allways a positive int
int modp(int a, int b) {
  return (a + b) % b;
}

// apply filter kernal to distance data (i.e. smooth the data)
double applyFilter(float nextDistance) {
  // update newest value
  distanceBuffer[nextPos] = nextDistance;

  double sum = 0.0;
  for (int i = 0; i < KERNAL_SIZE; i++) {
    int disti = modp(nextPos + i + 1, KERNAL_SIZE);
    sum += distanceBuffer[disti] * filterKernal[i] * 2;
  }

  // increment for next time
  nextPos = modp(nextPos + 1, KERNAL_SIZE);
  return sum;
}

void loop()
{
    double distance = MAX_OUTPUT_FREQENCY - (analogRead(inputPin) / 600.0 * MAX_OUTPUT_FREQENCY);
    double smoothDistance = applyFilter(distance);

    if (distance > 1200) {
      frequency = 0;
    } else {
      frequency = smoothDistance;
    }
    
    // // frequency 0 -> ~1000 Hz
    // if (smoothDistance > MAX_ULTRASONIC_DISTANCE) {
    //   frequency = 0;
    // } else {
    //   frequency = smoothDistance * 50.0 - 50;  // shift and scale
    //   frequency = min(frequency, MAX_OUTPUT_FREQENCY);
    // }


    clearBit (TIMSK2,TOIE2);              // disble Timer2 Interrupt
    tword_m = pow(2,32) * frequency / refclk;  // calulate DDS new tuning word
    setBit (TIMSK2,TOIE2);              // enable Timer2 Interrupt 

    // Serial.print(frequency);
    // Serial.print("  ");
    Serial.print(distance);
    Serial.print("  ");
    Serial.println(smoothDistance);

    delay(10);  // prevent lock up
 }

// timer2 setup
// set prscaler to 1, PWM mode to phase correct PWM,  16000000/510 = 31372.55 Hz clock
void Setup_timer2() {

// Timer2 Clock Prescaler to : 1
  setBit (TCCR2B, CS20);
  clearBit (TCCR2B, CS21);
  clearBit (TCCR2B, CS22);

  // Timer2 PWM Mode set to Phase Correct PWM
  clearBit (TCCR2A, COM2A0);  // clear Compare Match
  setBit (TCCR2A, COM2A1);

  // phase correct vs freqency correct?
  setBit (TCCR2A, WGM20);  // Mode 1  / Phase Correct PWM
  clearBit (TCCR2A, WGM21);
  clearBit (TCCR2B, WGM22);
}

//******************************************************************
// Timer2 Interrupt Service at 31372,550 KHz = 32uSec
// this is the timebase REFCLOCK for the DDS generator
// FOUT = (M (REFCLK)) / (2 exp 32)
// runtime : 8 microseconds ( inclusive push and pop)
ISR(TIMER2_OVF_vect) {
  phaccu = phaccu + tword_m; // soft DDS, phase accumulator with 32 bits
  icnt = phaccu >> 24;     // use upper 8 bits for phase accumulator as frequency information
                         // read value fron ROM sine table and send to PWM DAC
  OCR2A = pgm_read_byte_near(sine256 + icnt);
}