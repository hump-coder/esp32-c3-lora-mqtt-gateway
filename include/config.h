#pragma once

#include <Arduino.h>

// WiFi credentials
extern const char *WIFI_SSID;
extern const char *WIFI_PASSWORD;

// MQTT settings
extern const char *MQTT_HOST;
extern const uint16_t MQTT_PORT;
extern const char *MQTT_USER;
extern const char *MQTT_PASSWORD;

// LoRa radio pins (SX1262)
#define LORA_MOSI 6
#define LORA_MISO 5
#define LORA_SCK 4
#define LORA_CS 7
#define LORA_RST 8
#define LORA_BUSY 3
#define LORA_DIO1 9

// LoRa radio parameters
extern const float LORA_FREQ;
extern const float LORA_BW;
extern const uint8_t LORA_SF;
extern const uint8_t LORA_CR;
extern const uint8_t LORA_SYNC_WORD;

// Gateway stats publish interval (ms)
extern const uint32_t GATEWAY_STATS_INTERVAL;

