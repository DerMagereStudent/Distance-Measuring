#include <Arduino.h>

#include "Mpu6050/Mpu6050Sensor.h"

#define DEFAULT_BAUD_RATE 115200

Mpu6050Sensor mpuSensor;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(DEFAULT_BAUD_RATE);
  mpuSensor.initialize();
}

void loop() {
  // put your main code here, to run repeatedly:
  mpuSensor.updateOrientation();
  Mpu6050GyroData orientation = mpuSensor.getOrientation();

  Serial.print("Rotation X: ");
  Serial.print(orientation.x);
  Serial.print(", Y: ");
  Serial.print(orientation.y);
  Serial.print(", Z: ");
  Serial.print(orientation.z);
  Serial.println(" deg");
}