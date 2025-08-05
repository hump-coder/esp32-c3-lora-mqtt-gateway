#pragma once

#include <RadioLib.h>
#include <Arduino.h>

class LoRaHandler {
 public:
  LoRaHandler();
  int begin();
  bool sendPacket(const String &packet);
  bool available();
  String receivePacket(int &rssi, float &snr);

 private:
  Module* module;
  SX1262 radio;
};

