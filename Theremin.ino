// theremin.ino - Freqency measurment on Arduino Uno.
// ENEL200 Design Project, building a theremin.
// 25-09-17, Group 13

#include "freqy.h"
#include "timers.h"
#include "filter.h"

#define MAX_OUTPUT_FREQENCY 1500.0
#define MAX_RELYABLE_DISTANCE 120.0  // stop extreme readings from ultrasonics
#define MAX_ULTRASONIC_DISTANCE 100.0  // in cm

// get distance in cm to object
double get_ultrasonic_distance() {

    // send pulse to ultrasonic sensor
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2); 
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10); 
    digitalWrite(trigPin, LOW);

    // the time until a pulse is received from the echo pin
    unsigned long duration = pulseIn(echoPin, HIGH);

    // clip to MAX_RELYABLE_DISTANCE cm to stop extreme readings
    return min(duration / 58.2, MAX_RELYABLE_DISTANCE); // conversion of time to distance (in cm)
}

void setup()
{
    init_freqy();  // Get freqency measurer and generator ready
    init_filter_max_value()  // Start filter at the maximum value to stop transient beep

    Serial.begin(57600);
}

void loop() {
    double freqency_in = wait_and_capture_freqy(50);  // Wait 50 ms and read freqency from port 5
    double distance_in = apply_filter(get_ultrasonic_distance());

    Serial.print(distance_in)
    Serial.println(freqency_in);

    set_output_freqy(1000);  // generate 1000 Hz on port 11
}
