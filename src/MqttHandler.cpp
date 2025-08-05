#include "MqttHandler.h"
#include "config.h"

static const unsigned long ACTION_RETRY_INTERVAL = 5000;

MqttHandler *MqttHandler::instance = nullptr;

MqttHandler::MqttHandler(DeviceRegistry &registry, LoRaHandler &lora)
    : registry(registry), lora(lora), client(wifiClient), lastStats(0) {
  instance = this;
}

void MqttHandler::begin() {
  connectWifi();
  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(MqttHandler::onMessage);
  connectMqtt();
}

void MqttHandler::connectWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void MqttHandler::connectMqtt() {
  while (!client.connected()) {
    if (client.connect("lora-gateway", MQTT_USER, MQTT_PASSWORD)) {
      client.subscribe("lora/+/cmd");
    } else {
      delay(1000);
    }
  }
}

void MqttHandler::loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWifi();
  }
  if (!client.connected()) {
    connectMqtt();
  }
  client.loop();
  checkPending();
  unsigned long now = millis();
  if (now - lastStats > GATEWAY_STATS_INTERVAL) {
    publishGatewayStats(0, 0);
    lastStats = now;
  }
}

void MqttHandler::publishState(const String &topic, const String &payload) {
  client.publish(topic.c_str(), payload.c_str(), true);
}

void MqttHandler::publishGatewayStats(int rssi, float snr) {
  String payload = String("{") +
                   "\"devices\":" + registry.count() +
                   ",\"wifi_rssi\":" + WiFi.RSSI() +
                   ",\"last_rssi\":" + rssi +
                   ",\"last_snr\":" + snr +
                   ",\"uptime\":" + (millis() / 1000) +
                   "}";
  client.publish("lora/gateway/stats", payload.c_str(), true);
}

void MqttHandler::onMessage(char *topic, byte *payload, unsigned int length) {
  if (instance) {
    instance->handleMessage(topic, payload, length);
  }
}

void MqttHandler::handleMessage(char *topic, byte *payload, unsigned int length) {
  String t = String(topic);
  String data;
  for (unsigned int i = 0; i < length; i++) {
    data += (char)payload[i];
  }
  int first = t.indexOf('/');
  int second = t.indexOf('/', first + 1);
  String deviceId = t.substring(first + 1, second);
  String packet = deviceId + String(":cmd:") + data;
  bool sent = lora.sendPacket(packet);
  if (registry.isRegistered(deviceId) && registry.requiresAck(deviceId)) {
    PendingAction action{packet, sent ? millis() : 0};
    pending[deviceId]["cmd"] = action;
  }
  publishState(String("lora/") + deviceId + "/state", sent ? "cmd_sent" : "cmd_send_failed");
}

void MqttHandler::handleAck(const String &deviceId, const String &actionType) {
  auto devIt = pending.find(deviceId);
  if (devIt != pending.end()) {
    auto actIt = devIt->second.find(actionType);
    if (actIt != devIt->second.end()) {
      devIt->second.erase(actIt);
      if (devIt->second.empty()) {
        pending.erase(devIt);
      }
      publishState(String("lora/") + deviceId + "/state", actionType + "_ack");
    }
  }
}

void MqttHandler::checkPending() {
  unsigned long now = millis();
  for (auto &dev : pending) {
    for (auto &act : dev.second) {
      PendingAction &pa = act.second;
      if (now - pa.lastSent > ACTION_RETRY_INTERVAL) {
        bool sent = lora.sendPacket(pa.packet);
        pa.lastSent = sent ? now : 0;
      }
    }
  }
}

