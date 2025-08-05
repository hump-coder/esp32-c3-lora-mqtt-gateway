#pragma once
#include <Arduino.h>

class TemperatureSensorDevice {
 public:
  static String encode(float celsius) { return String(celsius, 2); }
  static float decode(const String &payload) { return payload.toFloat(); }
};

