#pragma once
#include <Arduino.h>

class MoistureSensorDevice {
 public:
  static String encode(int percent) { return String(percent); }
  static int decode(const String &payload) { return payload.toInt(); }
};

