#include <Arduino.h>
#include "LoRaHandler.h"
#include "GatewayProtocol.h"
#include "DeviceTypes/TemperatureSensorDevice.h"
#include "config.h"

LoRaHandler lora;
GatewayProtocol gateway(lora, "temp1", {"temperature"});

void setup() {
  Serial.begin(115200);
  lora.begin();
  gateway.begin();
}

void loop() {
  float temperatureC = 24.3;
  gateway.sendData("temperature", TemperatureSensorDevice::encode(temperatureC));
  gateway.loop();
  delay(10000);
}

