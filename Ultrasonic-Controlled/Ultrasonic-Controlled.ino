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
 */

#include "avr/pgmspace.h"
#include <math.h>

// 256 sine values, one sine period, stored in flash memory
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
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

double dfreq = 0.0;
// const double refclk=31372.549;  // =16MHz / 510
const double refclk=31376.6;      // measured

// variables used inside interrupt service declared as voilatile
volatile unsigned long phaccu;   // phase accumulator
volatile unsigned long tword_m;  // dds tuning word m

#define echoPin 7   // Echo Pin
#define trigPin 8   // Trigger Pin
#define PWMPin 11   // PWM Output
#define KERNAL_SIZE 13

double distanceBuffer[KERNAL_SIZE] = {0.0};
double filterKernal[KERNAL_SIZE] = {0.000244140625, 0.0029296875, 0.01611328125, 0.0537109375, 0.120849609375, 0.193359375, 0.2255859375, 0.193359375, 0.120849609375, 0.0537109375, 0.01611328125, 0.0029296875, 0.000244140625};
int nextPos = 3;

void setup()
{
  Serial.begin(115200);        // connect to the serial port
  Serial.println("DDS Test");

  pinMode(8, OUTPUT);      // sets the digital pin as output
  pinMode(7, INPUT);      // sets the digital pin as output
  pinMode(11, OUTPUT);     // pin11= PWM  output / frequency output

  Setup_timer2();

  // disable interrupts to avoid timing distortion
  cbi (TIMSK0,TOIE0);              // disable Timer0 !!! delay() is now not available
  sbi (TIMSK2,TOIE2);              // enable Timer2 Interrupt

  tword_m=pow(2,32)*dfreq/refclk;  // calulate DDS new tuning word 
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
    sum += distanceBuffer[disti] * filterKernal[i];
  }

  // increment for next time
  nextPos = modp(nextPos + 1, KERNAL_SIZE);
  return sum;
}

// get distance in cm to object
double getUltrasonicDistance() {
  // send pulse to ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);

  // the time until a pulse is received from the echo pin
  unsigned long duration = pulseIn(echoPin, HIGH);
  
  return duration / 58.2; // conversion of time to distance (in cm)
}

double distance = 0.0;  // Duration used to calculate distance
#define MAX_ULTRASONIC_DISTANCE 100  // in cm
#define MAX_OUTPUT_FREQENCY 1500.0

void loop()
{
  while(1) {
    
    distance = getUltrasonicDistance();
    double smoothDistance = applyFilter(distance);
    
    // dfreq 0 -> 1000 Hz
    if (smoothDistance > MAX_ULTRASONIC_DISTANCE) {
      dfreq = 0;
    } else {
      dfreq  smoothDistance * 50.0 - 50;  // shift and scale
      dfreq = min(dfreq, MAX_OUTPUT_FREQENCY);
    }

    cbi (TIMSK2,TOIE2);              // disble Timer2 Interrupt
    tword_m = pow(2,32) * dfreq / refclk;  // calulate DDS new tuning word
    sbi (TIMSK2,TOIE2);              // enable Timer2 Interrupt 

    Serial.print(dfreq);
    Serial.print("  ");
    Serial.print(smoothDistance);
    Serial.print("  ");
    Serial.println(distance);
  }
 }

// timer2 setup
// set prscaler to 1, PWM mode to phase correct PWM,  16000000/510 = 31372.55 Hz clock
void Setup_timer2() {

// Timer2 Clock Prescaler to : 1
  sbi (TCCR2B, CS20);
  cbi (TCCR2B, CS21);
  cbi (TCCR2B, CS22);

  // Timer2 PWM Mode set to Phase Correct PWM
  cbi (TCCR2A, COM2A0);  // clear Compare Match
  sbi (TCCR2A, COM2A1);

  sbi (TCCR2A, WGM20);  // Mode 1  / Phase Correct PWM
  cbi (TCCR2A, WGM21);
  cbi (TCCR2B, WGM22);
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