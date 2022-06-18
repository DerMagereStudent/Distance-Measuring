#include "HCSR04/HCSR04UltrasonicSensor.h"

HCSR04UltrasonicSensor::HCSR04UltrasonicSensor() {}
HCSR04UltrasonicSensor::~HCSR04UltrasonicSensor() {}

void HCSR04UltrasonicSensor::initialize() {
    pinMode(GPIO_PIN_HCSR04_TRIG, OUTPUT);
    pinMode(GPIO_PIN_HCSR04_ECHO, INPUT);
}

float HCSR04UltrasonicSensor::messureDistance() {
    // Clears the trigPin
    digitalWrite(GPIO_PIN_HCSR04_TRIG, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(GPIO_PIN_HCSR04_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(GPIO_PIN_HCSR04_TRIG, LOW);
    
    // Reads the echoPin, returns the sound wave travel time in microseconds
    unsigned long duration = pulseIn(GPIO_PIN_HCSR04_ECHO, HIGH);
    float durationS = duration * US_TO_S;

    return SPEED_OF_SOUND_M_S * durationS * M_TO_CM / 2;
}