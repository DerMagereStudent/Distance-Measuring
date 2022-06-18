#include "Mpu6050/Mpu6050Sensor.h"

Mpu6050Sensor::Mpu6050Sensor() : wire(0), orientation({0,0,0}), velocityDrift({0,0,0}), lastMillis(-1) {}
Mpu6050Sensor::~Mpu6050Sensor() {}

void Mpu6050Sensor::initialize() {
    if (!this->wire.begin(GPIO_PIN_GY521_SDA, GPIO_PIN_GY521_SCL))
        Serial.println("Failed to set GPIO pins for MPU6050");

    if (!this->mpu.begin(MPU6050_I2CADDR_DEFAULT, &this->wire))
        Serial.println("Failed to initialize MPU6050");
}

Mpu6050GyroData Mpu6050Sensor::readCurrentVelocity() {
    sensors_event_t gyroEvent, discardEvent;
    this->mpu.getEvent(&discardEvent, &gyroEvent, &discardEvent);

    return {
        gyroEvent.gyro.x,
        gyroEvent.gyro.y,
        gyroEvent.gyro.z
    };
}

Mpu6050GyroData Mpu6050Sensor::getOrientation() {
    return this->orientation;
}

void Mpu6050Sensor::updateOrientation() {
    unsigned long ms = millis();

    if (this->lastMillis < 0) {
        this->lastMillis = ms;
        return;
    }

    unsigned long deltaMs = ms - this->lastMillis;
    Mpu6050GyroData velocity = this->readCurrentVelocity();
    Mpu6050GyroData degVelocity = velocity * RAD_TO_DEG - this->velocityDrift;
    Mpu6050GyroData deltaDeg = degVelocity / 1000 * deltaMs;
    this->orientation = this->orientation + deltaDeg;
    this->normalizeOrientation();
    this->lastMillis = ms;
}

void Mpu6050Sensor::normalizeOrientation() {
    int count = sizeof(this->orientation) / sizeof(float);
    float* pOrientation = (float*)&this->orientation;

    for (int i = 0; i < count; i++) {
        while (pOrientation[i] < 0.0f)
            pOrientation[i] += 360.0f;

        while (pOrientation[i] >= 360.0f)
            pOrientation[i] -= 360.0f;
    }
}

void Mpu6050Sensor::calibrate() {
    for (int i = 0; i < NECESSARY_CALIBRATIONS; i++) {
        Mpu6050GyroData velocity = this->readCurrentVelocity() * RAD_TO_DEG;
        this->velocityDrift = (this->velocityDrift + velocity) / 2.0f;
        delay(10);
    }
}