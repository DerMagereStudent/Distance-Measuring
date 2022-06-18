#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>

#include "Mpu6050/Mpu6050Sensor.h"
#include "HCSR04/HCSR04UltrasonicSensor.h"

// contains wifi credentials
#include "WiFiCredentials.h"

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

DynamicJsonDocument jsonDataDoc(3072);
AsyncWebServer server(80);

void writeToJsonDataDoc(float direction, float distance);
void setupWebServer();

void setup() {
  Serial.begin(DEFAULT_BAUD_RATE);
  mpu6050Sensor.initialize();
  hcsr04Sensor.initialize();
  setupWebServer();

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

void writeToJsonDataDoc(float direction, float distance) {
  if (jsonDataDoc.size() < JSON_DATA_ARRAY_MAX_SIZE) {
    JsonObject item = jsonDataDoc.createNestedObject();
    item[JSON_DATA_ARRAY_DIRECTION_KEY] = direction;
    item[JSON_DATA_ARRAY_DISTANCE_KEY] = distance;
  } else {
    JsonObject item = jsonDataDoc[jsonDataInsertIndex];
    item[JSON_DATA_ARRAY_DIRECTION_KEY] = direction;
    item[JSON_DATA_ARRAY_DISTANCE_KEY] = distance;

    jsonDataInsertIndex++;

    if (jsonDataInsertIndex == JSON_DATA_ARRAY_MAX_SIZE) {
      jsonDataInsertIndex = 0;
    }
  }
}

void setupWebServer() {
  // connect to wifi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Wifi connecting...");

  while (WiFi.status() != WL_CONNECTED) {
      delay(100);
      Serial.println("Resetting due to Wifi not connecting...");
      //ESP.restart();
  }

  Serial.print("Wifi connected, IP address: ");
  Serial.println(WiFi.localIP());

  // accessable using http://distance-measuring.local
  MDNS.begin("distance-measuring");

  // configure cors
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");

  // configure routes
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    serializeJson(jsonDataDoc, *response);
    request->send(response);
  });

  server.onNotFound([](AsyncWebServerRequest *request) {
    if (request->method() == HTTP_OPTIONS)
      request->send(200);
    else
      request->send(404);
  });

  server.begin();
}