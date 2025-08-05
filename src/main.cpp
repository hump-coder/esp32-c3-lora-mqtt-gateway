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
static const unsigned long PRESENCE_TIMEOUT = 60000;

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
        bool requireAck = false;
        bool trackPresence = false;
        String type = payload;
        int comma = payload.indexOf(',');
        if (comma > 0) {
          type = payload.substring(0, comma);
          String flags = payload.substring(comma + 1);
          int start = 0;
          while (start >= 0) {
            int next = flags.indexOf(',', start);
            String flag = next == -1 ? flags.substring(start) : flags.substring(start, next);
            if (flag == "ack") {
              requireAck = true;
            } else if (flag == "presence") {
              trackPresence = true;
            }
            if (next == -1) break;
            start = next + 1;
          }
        }
        registry.registerDevice(deviceId, type, requireAck, trackPresence);
        mqtt.publishState(String("lora/") + deviceId + "/state", "registered");
        if (trackPresence) {
          mqtt.publishState(String("lora/") + deviceId + "/state", "connected");
        }
      } else if (msgType == "data") {
        if (registry.isRegistered(deviceId)) {
          if (registry.updateLastSeen(deviceId)) {
            mqtt.publishState(String("lora/") + deviceId + "/state", "connected");
          }
          mqtt.publishState(String("lora/") + deviceId + "/state", payload);
        }
      } else if (msgType == "ack") {
        if (registry.updateLastSeen(deviceId)) {
          mqtt.publishState(String("lora/") + deviceId + "/state", "connected");
        }
        mqtt.handleAck(deviceId, payload);
      }
      mqtt.publishGatewayStats(lastRSSI, lastSNR);
    }
  }

  unsigned long now = millis();
  for (const String &id : registry.getDeviceIds()) {
    if (registry.shouldTrackPresence(id) && registry.isConnected(id)) {
      if (now - registry.getLastSeen(id) > PRESENCE_TIMEOUT) {
        registry.setConnected(id, false);
        mqtt.publishState(String("lora/") + id + "/state", "disconnected");
      }
    }
  }
}

