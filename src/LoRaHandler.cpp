#include "LoRaHandler.h"
#include "config.h"

LoRaHandler::LoRaHandler()
    : module(new Module(LORA_CS, LORA_DIO1, LORA_RST, LORA_BUSY)),
      radio(module) {}

int LoRaHandler::begin() {
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);
  int state = radio.begin(LORA_FREQ, LORA_BW, LORA_SF, LORA_CR);
  if (state == RADIOLIB_ERR_NONE) {
    radio.setDio1Action(nullptr);
  }
  return state;
}

bool LoRaHandler::sendPacket(const String &packet) {
  String mutablePacket = packet;
  int state = radio.transmit(mutablePacket);
  return state == RADIOLIB_ERR_NONE;
}

bool LoRaHandler::available() {
  if (radio.getPacketLength() > 0) {
    return true;
  }
  return false;
}

String LoRaHandler::receivePacket(int &rssi, float &snr) {
  String str;
  radio.receive(str);
  rssi = radio.getRSSI();
  snr = radio.getSNR();
  return str;
}

