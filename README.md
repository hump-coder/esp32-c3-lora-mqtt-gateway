# ESP32-C3 LoRa ⇄ MQTT Gateway

This project implements a simple LoRa to MQTT gateway for an ESP32-C3 board
paired with an **SX1262** LoRa transceiver.  The gateway listens for LoRa
packets from remote devices, forwards their data to MQTT topics and relays
commands from MQTT back to the devices.  It also publishes basic diagnostic
information suitable for Home Assistant.

## Hardware wiring

RadioLib style wiring is used.  Connect the SX1262 module to the ESP32‑C3 as
shown below:

| SX1262 Pin | ESP32‑C3 GPIO |
|------------|---------------|
| NSS        | 7             |
| SCK        | 4             |
| MISO       | 5             |
| MOSI       | 6             |
| DIO1       | 9             |
| BUSY       | 3             |
| RST        | 8             |
| VCC        | 3V3           |
| GND        | GND           |

## Packet format

LoRa packets use a very small, colon separated layout:

```
DEVICE_ID:MSG_TYPE:PAYLOAD
```

Example registration packet from a temperature sensor:

```
temp1:register:temperature
```

Data packet from the same sensor:

```
temp1:data:23.5
```

## MQTT topics

For each registered device the gateway exposes two MQTT topics:

```
lora/<device_id>/state   – published telemetry from the device
lora/<device_id>/cmd     – subscribed commands sent to the device
```

Gateway diagnostics are published to:

```
lora/gateway/state
lora/gateway/stats
```

The `stats` topic publishes a JSON document containing WiFi RSSI, last LoRa
RSSI/SNR, uptime and number of registered devices.  Topics are retained so
Home Assistant can easily discover them.

## Building

1. Install [PlatformIO](https://platformio.org/install).
2. Rename `include/config.h` values with your WiFi and MQTT credentials if
   necessary.
3. Build the project:

   ```bash
   pio run
   ```

4. Upload to the board:

   ```bash
   pio run --target upload
   ```

Example sketches are provided in the `examples/` directory showing how a
remote temperature sensor registers itself and how a relay device listens for
commands over LoRa.

## Home Assistant integration

Use MQTT discovery or manually configure sensors and switches using the topics
above.  Gateway statistics are provided in JSON making it easy to track WiFi
signal strength, last packet RSSI/SNR, uptime and the number of registered
LoRa devices.

## License

This project is released under the MIT license.  See [LICENSE](LICENSE) for
more information.

