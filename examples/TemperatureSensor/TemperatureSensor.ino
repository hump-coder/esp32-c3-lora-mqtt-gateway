#include <Arduino.h>
#include "LoRaHandler.h"
#include "DeviceTypes/TemperatureSensorDevice.h"
#include "config.h"

LoRaHandler lora;

void setup() {
  Serial.begin(115200);
  lora.begin();
  // send registration packet: DEVICE_ID:MSG_TYPE:PAYLOAD
  lora.sendPacket("temp1:register:temperature");
}

void loop() {
  float temperatureC = 24.3;
  String payload = TemperatureSensorDevice::encode(temperatureC);
  lora.sendPacket(String("temp1:data:") + payload);
  delay(10000);
}

