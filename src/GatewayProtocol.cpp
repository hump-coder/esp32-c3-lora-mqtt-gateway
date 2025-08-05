#include "GatewayProtocol.h"

GatewayProtocol::GatewayProtocol(LoRaHandler &lora, const String &deviceId,
                                 const std::vector<String> &sensors,
                                 bool requireAck, bool trackPresence)
    : lora(lora),
      deviceId(deviceId),
      sensors(sensors),
      requireAck(requireAck),
      trackPresence(trackPresence),
      commandHandler(nullptr) {}

void GatewayProtocol::begin() {
  String payload;
  for (size_t i = 0; i < sensors.size(); ++i) {
    if (i > 0) {
      payload += "|";
    }
    payload += sensors[i];
  }
  if (requireAck || trackPresence) {
    payload += ",";
    bool first = true;
    if (requireAck) {
      payload += "ack";
      first = false;
    }
    if (trackPresence) {
      if (!first) {
        payload += ",";
      }
      payload += "presence";
    }
  }
  lora.sendPacket(deviceId + String(":register:") + payload);
}

void GatewayProtocol::onCommand(CommandHandler handler) {
  commandHandler = handler;
}

void GatewayProtocol::loop() {
  int rssi;
  float snr;
  if (!lora.available()) {
    return;
  }
  String pkt = lora.receivePacket(rssi, snr);
  int first = pkt.indexOf(':');
  int second = pkt.indexOf(':', first + 1);
  if (first <= 0 || second <= first) {
    return;
  }
  String id = pkt.substring(0, first);
  String type = pkt.substring(first + 1, second);
  String payload = pkt.substring(second + 1);
  if (id != deviceId || type != "cmd") {
    return;
  }
  int eq = payload.indexOf('=');
  String actionType = "cmd";
  String value = payload;
  String sensor;
  if (eq > 0) {
    sensor = payload.substring(0, eq);
    value = payload.substring(eq + 1);
    actionType = sensor;
  }
  if (commandHandler) {
    commandHandler(sensor, value);
  }
  if (requireAck) {
    lora.sendPacket(deviceId + String(":ack:") + actionType);
  }
}

void GatewayProtocol::sendData(const String &sensor, const String &value) {
  lora.sendPacket(deviceId + String(":data:") + sensor + "=" + value);
}

