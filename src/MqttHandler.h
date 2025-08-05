#pragma once

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <map>
#include "DeviceRegistry.h"
#include "LoRaHandler.h"

class MqttHandler {
 public:
  MqttHandler(DeviceRegistry &registry, LoRaHandler &lora);
  void begin();
  void loop();
  void publishState(const String &topic, const String &payload);
  void publishGatewayStats(int rssi, float snr);
  void handleAck(const String &deviceId, const String &actionType);

 private:
  DeviceRegistry &registry;
  LoRaHandler &lora;
  WiFiClient wifiClient;
  PubSubClient client;
  unsigned long lastStats;

  struct PendingAction {
    String packet;
    unsigned long lastSent;
  };
  std::map<String, std::map<String, PendingAction>> pending;

  static MqttHandler *instance;
  void connectWifi();
  void connectMqtt();
  static void onMessage(char *topic, byte *payload, unsigned int length);
  void handleMessage(char *topic, byte *payload, unsigned int length);
  void checkPending();
};

