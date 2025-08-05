#include <Arduino.h>
#include "LoRaHandler.h"
#include "DeviceTypes/RelayDevice.h"
#include "config.h"

LoRaHandler lora;
bool relayState = false;

void setup() {
  Serial.begin(115200);
  lora.begin();
  lora.sendPacket("relay1:register:relay");
}

void loop() {
  int rssi; float snr;
  if (lora.available()) {
    String pkt = lora.receivePacket(rssi, snr);
    int first = pkt.indexOf(':');
    int second = pkt.indexOf(':', first + 1);
    String msgType = pkt.substring(first + 1, second);
    String payload = pkt.substring(second + 1);
    if (msgType == "cmd") {
      relayState = RelayDevice::decode(payload);
      Serial.println(String("Relay state: ") + (relayState ? "ON" : "OFF"));
    }
  }
}

