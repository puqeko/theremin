// theremin.ino - Freqency measurment on Arduino Uno.
// ENEL200 Design Project, building a theremin.
// 25-09-17, Group 13

#include "freqy.h"
#include "timers.h"

void setup()
{
    init_freqy();
    set_output_freqy(1000.0);  // generate 1000 Hz on port 11
    Serial.begin(57600);


    // test wave
    pinMode(6, OUTPUT);
    analogWrite(6, 127);
}

void loop() {
    double freqency_in = wait_and_capture_freqy(50);  // Read freqency from port 5
    Serial.println(freqency_in);
}
