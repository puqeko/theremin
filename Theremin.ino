// theremin.ino - Freqency measurment on Arduino Uno.
// ENEL200 Design Project, building a theremin.
// 25-09-17, Group 13

// Output pitch to pin 11.
// Input capacitive ocsilator on pin 5.

#include "freqy.h"
#include "timers.h"
#include "filter.h"

#define MAX_OUTPUT_FREQUENCY 1760.0
#define MAX_ULTRASONIC_DISTANCE 60.0  // in cm (trim to this)
#define MAX_RELIABLE_DISTANCE 120.0  // stop extreme readings from ultrasonics
#define MAX_WAIT_TIME ((MAX_RELIABLE_DISTANCE * 2) / (100 * 340) * 1.2)

#define FET_PINCHOFF_VOLTAGE 3.3
#define ARDUINO_MAX_VOLTAGE_OUT 5
#define continuous 0
#define discrete 1

// I/O pins
#define VOLUME_PIN 6
#define CALIBRATION_BUTTON_PIN 8
#define MODE_BUTTON_PIN 9
#define POWER_SWITCH_PIN 10 
#define POWER_LED_PIN 3  // TODO: Get a pin for this.

// Ultrasonic pins
#define TRIGGER_PIN 12
#define ECHO_PIN 13

// So that this does not hog CPU when not connected.
// For debugging.
static bool is_ultrasonic_connected = true;

static double abs_min_freq = 30550;
static double abs_max_freq = 31400;
static double abs_min_vol = 0.0;
static double abs_max_vol = 0.0;
static double previous_vol = 0.0;
static bool is_power_on = true;
static bool current_mode;

// float minFreq = 30600.00;
// float maxFreq = 31360.00;
// float absMinFreq = 30550.00;
// float absMaxFreq = 31400.00;


// get distance in cm to object
double get_ultrasonic_distance() {

    // Don't hog cpu if not connected
    if (!is_ultrasonic_connected) return 0;

	// send pulse to ultrasonic sensor
	digitalWrite(TRIGGER_PIN, LOW);
	delayMicroseconds(2);
	digitalWrite(TRIGGER_PIN, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIGGER_PIN, LOW);

	// the time until a pulse is received from the echo pin
    unsigned long duration = pulseIn(ECHO_PIN, HIGH, MAX_WAIT_TIME);
    
    if (duration == 0) {  // No return signal received.
        duration = MAX_RELIABLE_DISTANCE;
    }

	// clip to MAX_RELIABLE_DISTANCE cm to stop extreme readings
	return min(duration / 58.2, MAX_RELIABLE_DISTANCE); // 58.2 does conversion of time to distance (in cm)
}


void setup()
{
	frequency_init();  // Get frequency measurer and generator ready.
	filter_init_with(MAX_RELIABLE_DISTANCE);  // Start filter at the maximum value to stop transient beep.

    pinMode(LED_BUILTIN, OUTPUT);
	pinMode(TRIGGER_PIN, OUTPUT);
	pinMode(ECHO_PIN, INPUT);
	pinMode(CALIBRATION_BUTTON_PIN, INPUT_PULLUP); 
    pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
    pinMode(POWER_SWITCH_PIN, INPUT_PULLUP);
    pinMode(POWER_LED_PIN, OUTPUT);

    
    
    // TODO: check inital power switch value.
    
	Serial.begin(250000);
}


double distance_to_frequency(double distance) {
    // Shift up 100 Hz so that it is more audiable.
    return distance / MAX_ULTRASONIC_DISTANCE * MAX_OUTPUT_FREQUENCY + 100;
}


char distance_to_volume(double distance) {
    // -2 because Ultrasonic useless within 2 cm.
    // 255 is max output value to pin.
    // 5v is max pin voltage.
    return ((distance - 2) / (MAX_ULTRASONIC_DISTANCE - 2) * (255 * FET_PINCHOFF_VOLTAGE / 5.0));
}




char frequency_to_volume(double frequency) { 
    double vol = ((frequency - abs_min_freq) / (abs_max_freq - abs_min_freq)) * ((frequency - abs_min_freq) / (abs_max_freq - abs_min_freq)) * 255.0;
    if (frequency > abs_max_freq || frequency < abs_min_freq) {
      vol = 0;
    }
    Serial.println(vol);
    vol = min(255, vol);
    vol = max(0, vol);
    //Serial.println(vol);
    return (char) vol;
}


double calibrate() {
    // Insert calibration (low) function.
    frequency_set(0);

    double low_freq = frequency_read(100);  // Wait 100 ms
    //digitalWrite(LED_BUILTIN, HIGH);
    //Serial.println("LOW");

    while (digitalRead(CALIBRATION_BUTTON_PIN)) continue;  // Wait for release.
    delay(100);
    while (!digitalRead(CALIBRATION_BUTTON_PIN)) continue;  // Wait for press again.
    
    // Insert calibration (high) function.
    double high_freq = frequency_read(100);  // Wait 100 ms
    //digitalWrite(LED_BUILTIN, LOW);
    //Serial.println("HIGH");

    // With 5% padding.
    abs_min_freq = (high_freq - low_freq) * 0.8 + low_freq;
    abs_max_freq = high_freq;
}

double discretization(double frequency_out) {
  
   //double frequency_table[37] = {220.00, 233.08, 246.94, 261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392.00, 415.30,
   //                              440.00, 466.16, 493.88, 523.25, 554.37, 587.33, 622.25, 659.25, 698.46, 739.99, 783.99, 830.61,
   //                              880.00, 932.33, 987.77, 1046.50, 1108.73, 1174.66, 1244.51, 1318.51, 1396.91, 1479.98, 1567.98, 1661.22,
   //                              1760.00};

   double frequency_table[25] = {220.00, 233.08, 246.94, 261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392.00, 415.30,
                                 440.00, 466.16, 493.88, 523.25, 554.37, 587.33, 622.25, 659.25, 698.46, 739.99, 783.99, 830.61,
                                 880.00};
   //int n = 37;
   int n = 25;
   int first, middle, last;

   first = 0;
   last = n - 1;
   middle = (first + last) / 2;

   frequency_out = (frequency_out / 1760.00) * (frequency_table[last] - frequency_table[first]) + frequency_table[first];
 
   while (first <= last) {
      if (frequency_table[middle] < frequency_out) {
         first = middle + 1;    
      } else {
         last = middle - 1;
      }
      middle = (first + last)/2;
   }
   return frequency_table[middle];   

}

void mode_toggle (void) {
  if (current_mode == continuous) {
      current_mode = discrete;
  } else {
      current_mode = continuous;
  }
}

void buttons() {

    is_power_on = !digitalRead(POWER_SWITCH_PIN);

    // Make it go quiet.
    if (!is_power_on) {
        frequency_set(0);
        digitalWrite(POWER_LED_PIN, LOW);
    }

    if (is_power_on) {
        digitalWrite(POWER_LED_PIN, HIGH);
        
        if (digitalRead(MODE_BUTTON_PIN)) {
            mode_toggle();
            while(!digitalRead(MODE_BUTTON_PIN)) continue;
        }

        // Volume control: Calibrate input frequency boundaries from capacitive sensor.
        if (digitalRead(CALIBRATION_BUTTON_PIN)) {
            calibrate();
            while (digitalRead(CALIBRATION_BUTTON_PIN)) continue;  // Wait for release.
        }
    }
}


void loop() {

    buttons();

    if (is_power_on) {
        double distance_raw = get_ultrasonic_distance();  // Undetermined wait for responce pulse.
        double distance_in = filter_apply(distance_raw);

        double frequency_out = distance_to_frequency(distance_in);
        double frequency_in = frequency_read(16);  // Sample at ~62 Hz (1000/16) from port 5.

        // TODO: Add discrete frequency here.
        if (current_mode == discrete) {
            frequency_out = discretization(frequency_out);
        }

        // Clip so that no sound occurs when out of range.
        if (distance_raw > MAX_ULTRASONIC_DISTANCE) {
            frequency_out = 0;
        }
        
        frequency_set(frequency_out);  // Generate on port 11. 
        char volume = frequency_to_volume(frequency_in);

        // (0 - 255 gives 0V - 5V, or low to high volume)
        analogWrite(VOLUME_PIN, volume);  // Set volume pin 6 at frequency-controlled voltage.

        // Debug
        // Serial.print(frequency_out);
        // Serial.print(" ");
        // Serial.println(frequency_in);
    }
}


// double frequency_to_duty_cycle (double frequency_in) {
//   double gate_voltage;
//   if (frequency_in < abs_min_frequency || frequency_in > abs_max_frequency) {
// 	gate_voltage = 0;
//   }
//   else {
// 	gate_voltage = frequency_in / abs_max_frequency * FET_PINCHOFF_VOLTAGE;
//   }
//   int duty_cycle = gate_voltage / ARDUINO_MAX_VOLTAGE_OUT;
//   return duty_cycle;
// }


