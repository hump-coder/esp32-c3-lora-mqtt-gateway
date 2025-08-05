#include "DeviceRegistry.h"

void DeviceRegistry::registerDevice(const String &id, const String &type) {
  devices[id] = {id, type};
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

