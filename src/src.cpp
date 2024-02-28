
#include <Arduino.h>
#include "define.h"

int ROBOT_ID;

// This is to hide non-test related source code.
// https://docs.platformio.org/en/latest/plus/unit-testing.html
#ifndef UNIT_TEST

void setup()
{
    // put your setup code here, to run once:

    // Enables Serial Communication with baudRate of 115200
    Serial.begin(115200);
    Serial.println("PeraSwarm Robot v5");

    memory.begin(); // NOTE: This should be run as the first thing.

    // This commands should be run 'ONLY' at the first run to assign a ID for robot
    // memory.setupRobotWithId(RobotId)
    // memory.setupRobotWithId(0);
    ROBOT_ID = memory.getRobotId();

    pinMode(PIN_LED_INBUILT, OUTPUT);

    // Scan available I2C devices
    i2c_scan();
}

void loop()
{
    // put your main code here, to run repeatedly:

    digitalWrite(PIN_LED_INBUILT, HIGH);
    delay(1000);
    digitalWrite(PIN_LED_INBUILT, LOW);
    delay(1000);
}

#endif