// theremin.ino - Freqency measurment on Arduino Uno.
// ENEL200 Design Project, building a theremin.
// 25-09-17, Group 13

// Output pitch to pin 11.
// Input capacitive ocsilator on pin 5.

#include "freqy.h"
#include "timers.h"
#include "filter.h"

#define MAX_OUTPUT_FREQENCY 1500.0
#define MAX_ULTRASONIC_DISTANCE 100.0  // in cm

// Ultrasonic readings
#define TRIGGER_PIN 8
#define ECHO_PIN 7
#define MAX_RELYABLE_DISTANCE 120.0  // stop extreme readings from ultrasonics

// get distance in cm to object
double get_ultrasonic_distance() {

    // send pulse to ultrasonic sensor
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2); 
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10); 
    digitalWrite(TRIGGER_PIN, LOW);

    // the time until a pulse is received from the echo pin
    unsigned long duration = pulseIn(ECHO_PIN, HIGH);

    // clip to MAX_RELYABLE_DISTANCE cm to stop extreme readings
    return min(duration / 58.2, MAX_RELYABLE_DISTANCE); // 58.2 does conversion of time to distance (in cm)
}

void setup()
{
    init_freqy();  // Get freqency measurer and generator ready
    init_filter_with(MAX_RELYABLE_DISTANCE);  // Start filter at the maximum value to stop transient beep

    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    Serial.begin(115200);
}

void loop() {
    double freqency_in = wait_and_capture_freqy(50);  // Wait 50 ms and read freqency from port 5
    double distance_in = get_ultrasonic_distance();


    Serial.print(distance_in);
    Serial.print(" ");
    Serial.println(freqency_in);

    set_output_freqy(1000);  // generate 1000 Hz on port 11
}
