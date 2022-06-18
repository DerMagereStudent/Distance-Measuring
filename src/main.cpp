#include <Arduino.h>
#include <ArduinoJson.h>

#include "Mpu6050/Mpu6050Sensor.h"
#include "HCSR04/HCSR04UltrasonicSensor.h"

#define DEFAULT_BAUD_RATE 115200

#define ORIENTATION_REFRESH_TIME_MS 10
#define DISTANCE_REFRESH_TIME_MS 500

#define JSON_DATA_ARRAY_MAX_SIZE 50
#define JSON_DATA_ARRAY_DIRECTION_KEY "direction"
#define JSON_DATA_ARRAY_DISTANCE_KEY "distance"

unsigned int globalTimestamp = 0;
unsigned int lastOrientationTimestamp = 0;
unsigned int lastDistanceTimestamp = 0;

unsigned int jsonDataInsertIndex = 0;

Mpu6050Sensor mpu6050Sensor;
HCSR04UltrasonicSensor hcsr04Sensor;

DynamicJsonDocument doc(3072);

void writeToJsonDataDoc(float direction, float distance) {
  if (doc.size() < JSON_DATA_ARRAY_MAX_SIZE) {
    JsonObject item = doc.createNestedObject();
    item[JSON_DATA_ARRAY_DIRECTION_KEY] = direction;
    item[JSON_DATA_ARRAY_DISTANCE_KEY] = distance;
  } else {
    JsonObject item = doc[jsonDataInsertIndex];
    item[JSON_DATA_ARRAY_DIRECTION_KEY] = direction;
    item[JSON_DATA_ARRAY_DISTANCE_KEY] = distance;

    jsonDataInsertIndex++;

    if (jsonDataInsertIndex == JSON_DATA_ARRAY_MAX_SIZE) {
      jsonDataInsertIndex = 0;
    }
  }
}

void setup() {
  Serial.begin(DEFAULT_BAUD_RATE);
  mpu6050Sensor.initialize();
  hcsr04Sensor.initialize();

  mpu6050Sensor.calibrate();
}

void loop() {
  globalTimestamp = millis();

  if (globalTimestamp - lastOrientationTimestamp > ORIENTATION_REFRESH_TIME_MS) {
    mpu6050Sensor.updateOrientation();
    lastOrientationTimestamp = globalTimestamp;
  }

  if (globalTimestamp - lastDistanceTimestamp > DISTANCE_REFRESH_TIME_MS) {
    Mpu6050GyroData orientation = mpu6050Sensor.getOrientation();
    float distance = hcsr04Sensor.messureDistance();
    writeToJsonDataDoc(orientation.z, distance);
    lastDistanceTimestamp = globalTimestamp;
  }
}