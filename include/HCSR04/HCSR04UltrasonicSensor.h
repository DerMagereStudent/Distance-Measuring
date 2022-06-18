#ifndef HC_SR04_ULTRASONIC_SENSOR
#define HC_SR04_ULTRASONIC_SENSOR

#include <Arduino.h>

#define GPIO_PIN_HCSR04_TRIG 26
#define GPIO_PIN_HCSR04_ECHO 27

#define SPEED_OF_SOUND_M_S 343
#define US_TO_S 0.000001f
#define M_TO_CM 100

class HCSR04UltrasonicSensor {
public:
    HCSR04UltrasonicSensor();
    ~HCSR04UltrasonicSensor();

    void initialize();
    float messureDistance();
};

#endif