#ifndef MPU_6050_SENSOR_H
#define MPU_6050_SENSOR_H

#include "Arduino.h"
#include <Adafruit_MPU6050.h>
#include <Wire.h>

#include "Mpu6050GyroData.h"

#define GPIO_PIN_GY521_SCL 22
#define GPIO_PIN_GY521_SDA 21

class Mpu6050Sensor {
public:
    Mpu6050Sensor();
    ~Mpu6050Sensor();

    void initialize();
    Mpu6050GyroData readCurrentVelocity();
    Mpu6050GyroData getOrientation();
    void updateOrientation();

private:
    void normalizeOrientation();

    TwoWire wire;
    Adafruit_MPU6050 mpu;
    Mpu6050GyroData orientation;
    long lastMillis;
};

#endif