#include "MqttHandler.h"
#include "config.h"

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
  lora.sendPacket(deviceId + String(":cmd:") + data);
  publishState(String("lora/") + deviceId + "/state", "cmd_sent");
}

