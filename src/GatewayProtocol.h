#pragma once

#include <Arduino.h>
#include <functional>
#include <vector>
#include "LoRaHandler.h"

class GatewayProtocol {
 public:
  using CommandHandler = std::function<void(const String &sensor, const String &value)>;

  GatewayProtocol(LoRaHandler &lora, const String &deviceId,
                  const std::vector<String> &sensors,
                  bool requireAck = false, bool trackPresence = false);

  void begin();
  void loop();
  void sendData(const String &sensor, const String &value);
  void onCommand(CommandHandler handler);

 private:
  LoRaHandler &lora;
  String deviceId;
  std::vector<String> sensors;
  bool requireAck;
  bool trackPresence;
  CommandHandler commandHandler;
};

