/* Theremin Test
 *
 * Therremin with TTL Oscillator 4MHz
 * Timer1 for freauency measurement
 * Timer2 for gate time
 * connect Oscillator on digital pin 5
 * connect Speaker with 1K Resistor in series on pin 8

 * KHM 2008 /  Martin Nawrath
 * Kunsthochschule fuer Medien Koeln
 * Academy of Media Arts Cologne

 */
 #include <Stdio.h>
 #include <math.h>
 #define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
 #define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
 
 //! Macro that clears all Timer/Counter1 interrupt flags.
 #define CLEAR_ALL_TIMER1_INT_FLAGS    (TIFR1 = TIFR1)
 
 int pinLed = 13;                 // LED connected to digital pin 13
 int pinFreq = 5;
 
 void setup()
 {
   pinMode(pinLed, OUTPUT);      // sets the digital pin as output
   pinMode(pinFreq, INPUT);
   pinMode(6, OUTPUT);
   analogWrite(6, 127);
 
   Serial.begin(57600);        // connect to the serial port
 
   // hardware counter setup ( refer atmega168.pdf chapter 16-bit counter1)
   TCCR1A=0;                   // reset timer/counter1 control register A
   TCCR1B=0;                   // reset timer/counter1 control register A
   TCNT1=0;                    // counter value = 0
   // set timer/counter1 hardware as counter , counts events on pin T1 ( arduino pin 5)
   // normal mode, wgm10 .. wgm13 = 0
   sbi (TCCR1B ,CS10);         // External clock source on T1 pin. Clock on rising edge.
   sbi (TCCR1B ,CS11);
   sbi (TCCR1B ,CS12);
 
   // timer2 setup / is used for frequency measurement gatetime generation
   // timer 2 presaler set to 256 / timer 2 clock = 16Mhz / 256 = 62500 Hz
   cbi (TCCR2B ,CS20);
   sbi (TCCR2B ,CS21);
   sbi (TCCR2B ,CS22);
 
   //set timer2 to CTC Mode
   cbi (TCCR2A ,WGM20);
   sbi (TCCR2A ,WGM21);
   cbi (TCCR2B ,WGM22);
   OCR2A = 124;                  // CTC at top of OCR2A / timer2 interrupt when coun value reaches OCR2A value
 
   // interrupt control
 
   sbi (TIMSK2,OCIE2A);          // enable Timer2 Interrupt
 
 }
 
 volatile byte i_tics;
 volatile byte f_ready ;
 volatile byte mlt ;
 unsigned int ww;
 
 int cal;
 int cal_max;
 
 char st1[32];
 long freq_in;
 long freq_zero;
 long freq_cal;
 
 unsigned int dds;
 int tune;
 
 int cnt=0;
 float voltageOut;
 float minFreq = 30600.00;
 float maxFreq = 31360.00;
 float absMinFreq = 30550.00;
 float absMaxFreq = 31400.00;
 
 void loop()
 {
   cnt++;
   // add=analogRead(0);
 
   f_meter_start();
   while (f_ready == 0);            // wait for period length end (100ms) by interrupt
   Serial.println(freq_in * 10);
 }
 //******************************************************************
 void f_meter_start() {
   f_ready=0;                      // reset period measure flag
   i_tics=0;                        // reset interrupt counter
   sbi (GTCCR,PSRASY);              // reset presacler counting
   TCNT2=0;                         // timer2=0
   TCNT1=0;                         // Counter1 = 0
   //cbi (TIMSK0,TOIE0);              // dissable Timer0 again // millis and delay
   sbi (TIMSK2,OCIE2A);             // enable Timer2 Interrupt
 }
 
 //******************************************************************
 // Timer2 Interrupt Service is invoked by hardware Timer2 every 2ms = 500 Hz
 //  16Mhz / 256 / 125 / 500 Hz
 // here the gatetime generation for freq. measurement takes place: 
 
 ISR(TIMER2_COMPA_vect) {
   if (i_tics==0) {
    TCCR1B = TCCR1B | 7;             //  Counter Clock source = pin T1 , start counting now
   }
   if (i_tics==50) {         // multiple 2ms = gate time = 100 ms
                             // end of gate time, measurement ready
     TCCR1B = TCCR1B & ~7;   // Gate Off  / Counter T1 stopped
     cbi (TIMSK2,OCIE2A);    // disable Timer2 Interrupt
     sbi (TIMSK0,TOIE0);     // ensable Timer0 again // millis and delay
     f_ready=1;              // set global flag for end count period
 
                             // calculate now frequeny value
     freq_in=0x10000 * mlt;  // mult #overflows by 65636
     freq_in += TCNT1;       // add counter1 value
     mlt=0;
 
   }
   i_tics++;                 // count number of interrupt events
   if (TIFR1 & 1) {          // if Timer/Counter 1 overflow flag
     mlt++;                  // count number of Counter1 overflows
     sbi(TIFR1,TOV1);        // clear Timer/Counter 1 overflow flag
   }
 
 }
