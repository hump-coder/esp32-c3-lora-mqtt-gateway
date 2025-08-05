#pragma once
#include <Arduino.h>

class RelayDevice {
 public:
  static String encode(bool on) { return on ? "ON" : "OFF"; }
  static bool decode(const String &payload) { return payload == "ON"; }
};

