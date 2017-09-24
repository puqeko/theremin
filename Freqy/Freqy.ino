#include "freqy.h"
#include "timers.h"

void setup()
{
    init_freqy();
    set_output_freqy(1000.0);
    Serial.begin(57600);
}

void loop() {
    double freqency_in = get_input_freqy();
    Serial.println(freqency_in);
}