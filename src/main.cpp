#include <Arduino.h>

#include "Mpu6050/Mpu6050Sensor.h"
#include "HCSR04/HCSR04UltrasonicSensor.h"

#define DEFAULT_BAUD_RATE 115200

Mpu6050Sensor mpuSensor;
HCSR04UltrasonicSensor hcsr04Sensor;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(DEFAULT_BAUD_RATE);
  mpuSensor.initialize();
  hcsr04Sensor.initialize();

  mpuSensor.calibrate();
}

void loop() {
  // put your main code here, to run repeatedly:
  mpuSensor.updateOrientation();
  Mpu6050GyroData orientation = mpuSensor.getOrientation();

  Serial.println(orientation.toString(4));
  Serial.println(String(hcsr04Sensor.messureDistance(), 4));

  delay(100);
}