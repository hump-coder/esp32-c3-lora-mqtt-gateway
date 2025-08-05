#pragma once

#include <Arduino.h>
#include <map>

struct RegisteredDevice {
  String id;
  String type;
  bool requiresAck;
};

class DeviceRegistry {
 public:
  void registerDevice(const String &id, const String &type, bool requiresAck = false);
  bool isRegistered(const String &id) const;
  String getType(const String &id) const;
  bool requiresAck(const String &id) const;
  size_t count() const { return devices.size(); }

 private:
  std::map<String, RegisteredDevice> devices;
};

