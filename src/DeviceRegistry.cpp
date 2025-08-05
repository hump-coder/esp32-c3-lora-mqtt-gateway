#include "DeviceRegistry.h"

void DeviceRegistry::registerDevice(const String &id, const String &type, bool requiresAck) {
  devices[id] = {id, type, requiresAck};
}

bool DeviceRegistry::isRegistered(const String &id) const {
  return devices.find(id) != devices.end();
}

String DeviceRegistry::getType(const String &id) const {
  auto it = devices.find(id);
  if (it != devices.end()) {
    return it->second.type;
  }
  return String();
}

bool DeviceRegistry::requiresAck(const String &id) const {
  auto it = devices.find(id);
  if (it != devices.end()) {
    return it->second.requiresAck;
  }
  return false;
}

