#include "DeviceRegistry.h"

void DeviceRegistry::registerDevice(const String &id,
                                    const std::vector<String> &sensors,
                                    bool requiresAck, bool trackPresence) {
  RegisteredDevice dev{id, sensors, requiresAck, trackPresence, true,
                       millis()};
  devices[id] = dev;
}

bool DeviceRegistry::isRegistered(const String &id) const {
  return devices.find(id) != devices.end();
}

std::vector<String> DeviceRegistry::getSensors(const String &id) const {
  auto it = devices.find(id);
  if (it != devices.end()) {
    return it->second.sensors;
  }
  return {};
}

bool DeviceRegistry::requiresAck(const String &id) const {
  auto it = devices.find(id);
  if (it != devices.end()) {
    return it->second.requiresAck;
  }
  return false;
}

bool DeviceRegistry::shouldTrackPresence(const String &id) const {
  auto it = devices.find(id);
  if (it != devices.end()) {
    return it->second.trackPresence;
  }
  return false;
}

bool DeviceRegistry::isConnected(const String &id) const {
  auto it = devices.find(id);
  if (it != devices.end()) {
    return it->second.connected;
  }
  return false;
}

unsigned long DeviceRegistry::getLastSeen(const String &id) const {
  auto it = devices.find(id);
  if (it != devices.end()) {
    return it->second.lastSeen;
  }
  return 0;
}

bool DeviceRegistry::updateLastSeen(const String &id) {
  auto it = devices.find(id);
  if (it != devices.end()) {
    it->second.lastSeen = millis();
    if (it->second.trackPresence && !it->second.connected) {
      it->second.connected = true;
      return true;
    }
  }
  return false;
}

void DeviceRegistry::setConnected(const String &id, bool connected) {
  auto it = devices.find(id);
  if (it != devices.end()) {
    it->second.connected = connected;
  }
}

std::vector<String> DeviceRegistry::getDeviceIds() const {
  std::vector<String> ids;
  for (const auto &pair : devices) {
    ids.push_back(pair.first);
  }
  return ids;
}

