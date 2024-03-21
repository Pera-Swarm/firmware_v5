/**
 * @brief    Swarm Robot Motor Controller Library
 * @author   Nuwan Jaliyagoda
 * @version  2.0.0
 * @url      N/A
 *
 * ------------------------------------------------------------------------------
 */

#include "robot_motors.h"
#include "config/pins.h"

#define LEDC_RESOLUTION_BITS 8
#define LEDC_BASE_FREQ 5000
#define LEDC_CHANNEL_A 8
#define LEDC_CHANNEL_B 9

/*
 * Motor module
 */
SW_Motors::SW_Motors()
{
}

SW_Motors::~SW_Motors()
{
    ledcDetachPin(PIN_PWM_R);
    ledcDetachPin(PIN_PWM_L);
}

/**
 * Setup the motor module
 */
void SW_Motors::begin()
{
    // TODO: Try drive the robot using 1 directional pin per motor
    pinMode(PIN_MOT_R1, OUTPUT);
    pinMode(PIN_MOT_R2, OUTPUT);
    pinMode(PIN_MOT_L1, OUTPUT);
    pinMode(PIN_MOT_L2, OUTPUT);

    ledcSetup(LEDC_CHANNEL_A, LEDC_BASE_FREQ, LEDC_RESOLUTION_BITS);
    ledcSetup(LEDC_CHANNEL_B, LEDC_BASE_FREQ, LEDC_RESOLUTION_BITS);

    ledcAttachPin(PIN_PWM_R, LEDC_CHANNEL_A);
    ledcAttachPin(PIN_PWM_L, LEDC_CHANNEL_B);

    ledcWrite(LEDC_CHANNEL_A, 0);
    ledcWrite(LEDC_CHANNEL_B, 0);

    Serial.println(">> Motors\t:enabled,pwm");

    this->write(0, 0);
}

/**
 * Core motor controlling function
 * @param leftSpeed speed in range [-MAX_MOTOR_SPEED, MAX_MOTOR_SPEED]
 * @param rightSpeed speed in range [-MAX_MOTOR_SPEED, MAX_MOTOR_SPEED]
 */
void SW_Motors::write(int16_t leftSpeed, int16_t rightSpeed)
{
    // Adjustment to remove the drift
    // TODO implement a linear correction
    if (leftSpeed > 30)
        leftSpeed += leftCorrection;
    else if (leftSpeed < -30)
        leftSpeed -= leftCorrection;

    if (rightSpeed > 30)
        rightSpeed += rightCorrection;
    else if (rightSpeed < -30)
        rightSpeed -= rightCorrection;

    // map the speed with correct & acceptable range
    leftSpeed = constrain(leftSpeed, -1 * MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);
    rightSpeed = constrain(rightSpeed, -1 * MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);

    // Serial.printf("M: %d %d\n", leftSpeed, rightSpeed);

    // motor rotating directions
    this->leftMotorDir = (leftSpeed >= 0) ? 1 : 0;
    this->rightMotorDir = (rightSpeed >= 0) ? 1 : 0;

    // check motor directions
    if (this->leftMotorDir != this->leftMotorDirOld)
    {
        // Direction changed
        digitalWrite(PIN_MOT_L1, (this->leftMotorDir) ? LOW : HIGH);
        digitalWrite(PIN_MOT_L2, (this->leftMotorDir) ? HIGH : LOW);
        this->leftMotorDirOld = this->leftMotorDir;
    }
    if (this->rightMotorDir != this->rightMotorDirOld)
    {
        // Direction changed
        digitalWrite(PIN_MOT_R1, (this->rightMotorDir) ? LOW : HIGH);
        digitalWrite(PIN_MOT_R2, (this->rightMotorDir) ? HIGH : LOW);
        this->rightMotorDirOld = this->rightMotorDir;
    }

    this->rightMotorSpeed = abs(rightSpeed);
    this->leftMotorSpeed = abs(leftSpeed);

    // Analog write function for ESP32
    ledcWrite(LEDC_CHANNEL_A, this->leftMotorSpeed);
    ledcWrite(LEDC_CHANNEL_B, this->rightMotorSpeed);
}

/**
 * Stop motors immediately
 */
void SW_Motors::stop()
{
    this->write(0, 0);
}

/**
 * Stop motors after a delay
 * @param delay delay in milliseconds
 */
void SW_Motors::stop(int16_t d)
{
    delay(d);
    this->write(0, 0);
}

/**
 * Pause motors for while
 */
void SW_Motors::pause()
{
    ledcWrite(LEDC_CHANNEL_A, 0);
    ledcWrite(LEDC_CHANNEL_B, 0);
}

/**
 * Resume motors
 */
void SW_Motors::resume()
{
    ledcWrite(LEDC_CHANNEL_A, this->leftMotorSpeed);
    ledcWrite(LEDC_CHANNEL_B, this->rightMotorSpeed);
}

/**
 * A test function to test motors
 */
void SW_Motors::test()
{
    // Clockwise rotation
    Serial.println(F("robot: CCW"));
    this->write(-200, 200);
    delay(500);
    this->stop(1500);

    // Counter Clockwise rotation
    Serial.println(F("robot: CW"));
    this->write(200, -200);
    delay(500);
    this->stop(1500);

    // Forward movement
    Serial.println(F("robot: forward++"));
    for (int i = 0; i < 255; i++)
    {
        this->write(i, i);
        delay(25);
    }
    delay(500);

    Serial.println(F("robot: forward--"));
    for (int i = 255; i > 0; i--)
    {
        this->write(i, i);
        delay(25);
    }
    this->stop(500);
    delay(2000);

    // Backward movement
    Serial.println(F("robot: backward++"));
    for (int i = 0; i < 255; i++)
    {
        this->write(-i, -i);
        delay(25);
    }
    delay(500);

    Serial.println(F("robot: backward--"));
    for (int i = 255; i > 0; i--)
    {
        this->write(-i, -i);
        delay(25);
    }
    this->stop(500);
    delay(2000);
}
