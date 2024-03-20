
#include <Arduino.h>
#include "define.h"

int ROBOT_ID;

// This is to hide non-test related source code.
// https://docs.platformio.org/en/latest/plus/unit-testing.html
#ifndef UNIT_TEST

void setup()
{
    Serial.begin(115200);
    Wire.begin();
    delay(2000);

    motion.begin();
}

void loop()
{
    static uint32_t prev_ms = millis();
    if (millis() > prev_ms + 25)
    {
        motion.test();
        prev_ms = millis();
    }
}

#endif