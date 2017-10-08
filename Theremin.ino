// theremin.ino - Freqency measurment on Arduino Uno.
// ENEL200 Design Project, building a theremin.
// 25-09-17, Group 13

// Output pitch to pin 11.
// Input capacitive ocsilator on pin 5.

#include "freqy.h"
#include "timers.h"
#include "filter.h"

#define MAX_OUTPUT_FREQUENCY 1500.0
#define MAX_ULTRASONIC_DISTANCE 60.0  // in cm

// Ultrasonic readings
#define TRIGGER_PIN 12
#define ECHO_PIN 13
#define MAX_RELIABLE_DISTANCE 120.0  // stop extreme readings from ultrasonics

#define CALIBRATION_BUTTON_PIN 8
#define FET_PINCHOFF_VOLTAGE 3.3
#define ARDUINO_MAX_VOLTAGE_OUT 5

#define VOLUME_PIN 6  // Timer 0
#define MODE_BUTTON_PIN 9

// #define NUMBER_OF_MODES 2
// enum modes {continuous, discrete};
// int current_mode = continuous;

bool isUltrasonicConnected = true;


// get distance in cm to object
double get_ultrasonic_distance(void) {

    // Don't hog cpu if not connected
    if (!isUltrasonicConnected) return 0;

	// send pulse to ultrasonic sensor
	digitalWrite(TRIGGER_PIN, LOW);
	delayMicroseconds(2);
	digitalWrite(TRIGGER_PIN, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIGGER_PIN, LOW);

	// the time until a pulse is received from the echo pin
	unsigned long duration = pulseIn(ECHO_PIN, HIGH);

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
	pinMode(8, INPUT_PULLUP); 
    pinMode(9, INPUT_PULLUP);
    
    // So that this does not hog CPU when not connect.
    //isUltrasonicConnected = (get_ultrasonic_distance() != 0);
    
	Serial.begin(250000);
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

// void mode_toggle (void) {
//   if (current_mode == continuous) {
// 	current_mode = discrete;
//   } else {
// 	current_mode = continuous;
//   }
// }


double distance_to_frequency (double distance) {
    return distance / MAX_ULTRASONIC_DISTANCE * MAX_OUTPUT_FREQUENCY + 100;
}


double distance_to_volume (double distance) {
    return ((distance - 2) / (MAX_ULTRASONIC_DISTANCE - 2) * (255 * FET_PINCHOFF_VOLTAGE / 5.0));
}


void buttons(void) {
    if (!digitalRead(MODE_BUTTON_PIN)) {
        //mode_toggle();
        while(!digitalRead(MODE_BUTTON_PIN)) continue;
    }

    // Volume control: Calibrate input frequency boundaries from capacitive sensor.
    if (!digitalRead(CALIBRATION_BUTTON_PIN)) {
        // Insert calibration (low) function.
        digitalWrite(LED_BUILTIN, HIGH);

        while (!digitalRead(CALIBRATION_BUTTON_PIN)) continue;  // Wait for release.
        while (digitalRead(CALIBRATION_BUTTON_PIN)) continue;  // Wait for press again.
        
        // Insert calibration (high) function.
        digitalWrite(LED_BUILTIN, LOW);

        while (!digitalRead(CALIBRATION_BUTTON_PIN)) continue;  // Wait for release.
    }
}

void loop() {

    buttons();

    double distance_raw = get_ultrasonic_distance();  // Undetermined wait for responce pulse.
    double distance_in = filter_apply(distance_raw);

    double frequency_out = distance_to_frequency(distance_in);
    double frequency_in = frequency_read(16);  // Sample at ~62 Hz (1000/16) from port 5.

    // Clip so that no sound occurs when out of range.
    if (distance_raw > MAX_ULTRASONIC_DISTANCE) {
        frequency_out = 0;
    }

    frequency_set(frequency_out);  // Generate 1000 Hz on port 11.

    // (0 - 255 gives 0V - 5V, or low to high volume)
    analogWrite(VOLUME_PIN, 255);  // Set volume pin 6 at frequency-controlled voltage.

    // Debug
    Serial.println(frequency_in);
}