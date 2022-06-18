#ifndef MPU_6050_SENSOR_H
#define MPU_6050_SENSOR_H

#include "Arduino.h"
#include <Adafruit_MPU6050.h>
#include <Wire.h>

#include "Mpu6050GyroData.h"

#define GPIO_PIN_GY521_SCL 22
#define GPIO_PIN_GY521_SDA 21

#define NECESSARY_CALIBRATIONS 1000

class Mpu6050Sensor {
public:
    Mpu6050Sensor();
    ~Mpu6050Sensor();

    void initialize();
    Mpu6050GyroData readCurrentVelocity();
    Mpu6050GyroData getOrientation();
    void updateOrientation();
    void calibrate();

private:
    void normalizeOrientation();

    TwoWire wire;
    Adafruit_MPU6050 mpu;
    Mpu6050GyroData velocityDrift;
    Mpu6050GyroData orientation;
    long lastMillis;
};

#endif