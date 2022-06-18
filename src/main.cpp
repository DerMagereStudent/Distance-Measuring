#include <Arduino.h>

#include "Mpu6050/Mpu6050Sensor.h"

#define DEFAULT_BAUD_RATE 115200

Mpu6050Sensor mpuSensor;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(DEFAULT_BAUD_RATE);
  mpuSensor.initialize();
  mpuSensor.calibrate();
}

void loop() {
  // put your main code here, to run repeatedly:
  mpuSensor.updateOrientation();
  Mpu6050GyroData orientation = mpuSensor.getOrientation();

  Serial.println(orientation.toString());

  delay(100);
}