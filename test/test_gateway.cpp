#include <Arduino.h>
#include <unity.h>
#include "DeviceRegistry.h"
#include "DeviceTypes/TemperatureSensorDevice.h"
#include "DeviceTypes/RelayDevice.h"

void test_device_registration() {
  DeviceRegistry reg;
  String pkt = "temp1:register:temperature";
  int first = pkt.indexOf(':');
  int second = pkt.indexOf(':', first + 1);
  String id = pkt.substring(0, first);
  String type = pkt.substring(second + 1);
  reg.registerDevice(id, type);
  TEST_ASSERT_TRUE(reg.isRegistered("temp1"));
  TEST_ASSERT_EQUAL_STRING("temperature", reg.getType("temp1").c_str());
}

void test_temperature_payload() {
  float t = 23.5f;
  String encoded = TemperatureSensorDevice::encode(t);
  TEST_ASSERT_FLOAT_WITHIN(0.01f, t, TemperatureSensorDevice::decode(encoded));
}

void test_relay_payload() {
  String encoded = RelayDevice::encode(true);
  TEST_ASSERT_TRUE(RelayDevice::decode(encoded));
  String packet = String("relay1:cmd:") + encoded;
  TEST_ASSERT_EQUAL_STRING("relay1:cmd:ON", packet.c_str());
}

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_device_registration);
  RUN_TEST(test_temperature_payload);
  RUN_TEST(test_relay_payload);
  UNITY_END();
}

void loop() {}

