#include "config.h"

// Update these values for your network
const char *WIFI_SSID = "YOUR_WIFI_SSID";
const char *WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

const char *MQTT_HOST = "mqtt.example.com";
const uint16_t MQTT_PORT = 1883;
const char *MQTT_USER = "mqtt_user";
const char *MQTT_PASSWORD = "mqtt_pass";

const float LORA_FREQ = 915.5;
const float LORA_BW = 125.0;
const uint8_t LORA_SF = 9;
const uint8_t LORA_CR = 5;
const uint8_t LORA_SYNC_WORD = 0x34;

const uint32_t GATEWAY_STATS_INTERVAL = 60000; // 60s

