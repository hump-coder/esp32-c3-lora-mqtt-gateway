#include <Arduino.h>
#include "LoRaHandler.h"
#include "GatewayProtocol.h"
#include "DeviceTypes/RelayDevice.h"
#include "config.h"

LoRaHandler lora;
GatewayProtocol gateway(lora, "relay1", {"relay"}, true);
bool relayState = false;

void setup() {
  Serial.begin(115200);
  lora.begin();
  gateway.onCommand([](const String &sensor, const String &value) {
    if (sensor == "relay") {
      relayState = RelayDevice::decode(value);
      Serial.println(String("Relay state: ") + (relayState ? "ON" : "OFF"));
    }
  });
  gateway.begin();
}

void loop() {
  gateway.loop();
}

