#include <Arduino.h>
#include "config.h"
#include "LoRaHandler.h"
#include "MqttHandler.h"
#include "DeviceRegistry.h"

LoRaHandler lora;
DeviceRegistry registry;
MqttHandler mqtt(registry, lora);

int lastRSSI = 0;
float lastSNR = 0.0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("LoRa MQTT Gateway starting");

  if (lora.begin() != RADIOLIB_ERR_NONE) {
    Serial.println("Failed to initialize LoRa radio");
    while (true) {
      delay(1000);
    }
  }

  mqtt.begin();
}

void loop() {
  mqtt.loop();

  if (lora.available()) {
    String packet = lora.receivePacket(lastRSSI, lastSNR);
    int first = packet.indexOf(':');
    int second = packet.indexOf(':', first + 1);
    if (first > 0 && second > first) {
      String deviceId = packet.substring(0, first);
      String msgType = packet.substring(first + 1, second);
      String payload = packet.substring(second + 1);

      if (msgType == "register") {
        registry.registerDevice(deviceId, payload); // payload contains device type
        mqtt.publishState(String("lora/") + deviceId + "/state", "registered");
      } else if (msgType == "data") {
        if (registry.isRegistered(deviceId)) {
          mqtt.publishState(String("lora/") + deviceId + "/state", payload);
        }
      }
      mqtt.publishGatewayStats(lastRSSI, lastSNR);
    }
  }
}

