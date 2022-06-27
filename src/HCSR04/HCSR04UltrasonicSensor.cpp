#include "HCSR04/HCSR04UltrasonicSensor.h"

HCSR04UltrasonicSensor::HCSR04UltrasonicSensor() {}
HCSR04UltrasonicSensor::~HCSR04UltrasonicSensor() {}

void HCSR04UltrasonicSensor::initialize() {
    pinMode(GPIO_PIN_HCSR04_TRIG, OUTPUT);
    pinMode(GPIO_PIN_HCSR04_ECHO, INPUT);
}

float HCSR04UltrasonicSensor::measureDistance() {
    // set pin to LOW in case it was changed or the intial state is not LOW
    digitalWrite(GPIO_PIN_HCSR04_TRIG, LOW);
    delayMicroseconds(2);

    digitalWrite(GPIO_PIN_HCSR04_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(GPIO_PIN_HCSR04_TRIG, LOW);
    
    unsigned long durationUs = pulseIn(GPIO_PIN_HCSR04_ECHO, HIGH);
    float durationS = durationUs * US_TO_S;

    return SPEED_OF_SOUND_M_S * durationS * M_TO_CM / 2;
}