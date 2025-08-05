#pragma once

#include <Arduino.h>
#include <map>
#include <vector>

struct RegisteredDevice {
  String id;
  String type;
  bool requiresAck;
  bool trackPresence;
  bool connected;
  unsigned long lastSeen;
};

class DeviceRegistry {
 public:
  void registerDevice(const String &id, const String &type, bool requiresAck = false,
                      bool trackPresence = false);
  bool isRegistered(const String &id) const;
  String getType(const String &id) const;
  bool requiresAck(const String &id) const;
  bool shouldTrackPresence(const String &id) const;
  bool isConnected(const String &id) const;
  unsigned long getLastSeen(const String &id) const;
  bool updateLastSeen(const String &id);
  void setConnected(const String &id, bool connected);
  std::vector<String> getDeviceIds() const;
  size_t count() const { return devices.size(); }

 private:
  std::map<String, RegisteredDevice> devices;
};

