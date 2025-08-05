#include <Arduino.h>
#include <unity.h>
#include <vector>
#include "DeviceRegistry.h"
#include "DeviceTypes/TemperatureSensorDevice.h"
#include "DeviceTypes/RelayDevice.h"

void test_device_registration() {
  DeviceRegistry reg;
  std::vector<String> sensors{"temperature", "battery"};
  reg.registerDevice("temp1", sensors);
  TEST_ASSERT_TRUE(reg.isRegistered("temp1"));
  auto regsensors = reg.getSensors("temp1");
  TEST_ASSERT_EQUAL(2, regsensors.size());
  TEST_ASSERT_EQUAL_STRING("temperature", regsensors[0].c_str());
  TEST_ASSERT_EQUAL_STRING("battery", regsensors[1].c_str());
}

void test_device_registration_ack() {
  DeviceRegistry reg;
  reg.registerDevice("dev1", std::vector<String>{"relay"}, true);
  TEST_ASSERT_TRUE(reg.requiresAck("dev1"));
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

void test_presence_tracking() {
  DeviceRegistry reg;
  reg.registerDevice("dev1", std::vector<String>{"relay"}, false, true);
  TEST_ASSERT_TRUE(reg.shouldTrackPresence("dev1"));
  TEST_ASSERT_TRUE(reg.isConnected("dev1"));
  reg.setConnected("dev1", false);
  TEST_ASSERT_FALSE(reg.isConnected("dev1"));
  bool reconnected = reg.updateLastSeen("dev1");
  TEST_ASSERT_TRUE(reconnected);
  TEST_ASSERT_TRUE(reg.isConnected("dev1"));
}

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_device_registration);
  RUN_TEST(test_device_registration_ack);
  RUN_TEST(test_temperature_payload);
  RUN_TEST(test_relay_payload);
  RUN_TEST(test_presence_tracking);
  UNITY_END();
}

void loop() {}

