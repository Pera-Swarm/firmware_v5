
#include <Arduino.h>
#include "define.h"

int ROBOT_ID;

// This is to hide non-test related source code.
// https://docs.platformio.org/en/latest/plus/unit-testing.html
#ifndef UNIT_TEST

void setup()
{
    Serial.begin(115200);
    motion.begin();
    
    // motion.calibrate();
    
    memory.begin();
    // memory.saveCalibration();
    memory.loadCalibration();

}

void loop()
{
    motion.test();
}

#endif