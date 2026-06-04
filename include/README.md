# Water Level Monitor v3.0

ESP32 + P35 Float Switches + SIM800L GSM + Blynk IoT

## Quick start

1. Install libraries in Arduino IDE → Manage Libraries:
   - `Blynk` by Volodymyr Shymanskyy
   - `ArduinoJson` by Benoit Blanchon

2. Edit `src/config/app_config.h`:
   - WiFi SSID / password
   - Blynk Template ID, Template Name, Auth Token
   - Phone number for SMS

3. Edit `src/config/sensor_config.h` if your P35 is NC type:
   - Change `SENSOR_ACTIVE_STATE` from `HIGH` to `LOW`

4. Open `src/main.c` in Arduino IDE (or add all `.c/.h` files to your sketch)

5. Flash to ESP32

## Blynk app setup

Create these datastreams in blynk.cloud:

| Virtual pin | Type | Range | Widget |
|---|---|---|---|
| V0 | Integer | 0–100 | Gauge |
| V1 | String | — | Label |
| V2 | Integer | 0–1 | Button |
| V3 | Integer | 0–1 | Switch |
| V4 | String | — | Terminal |
| V5 | String | — | Label |

## Running tests

```bash
gcc -o test_sensor test/test_sensor_scaling.c && ./test_sensor
gcc -o test_sms    test/test_sms_cooldown.c   && ./test_sms
```

## Wiring

| Component | ESP32 pin | Notes |
|---|---|---|
| P35 HIGH sensor | GPIO34 | + 10kΩ to GND |
| P35 LOW sensor | GPIO35 | + 10kΩ to GND |
| Relay | GPIO26 | Active-low optocoupler |
| SIM800L TX → | GPIO16 (RX2) | |
| SIM800L RX ← | GPIO17 (TX2) | |

See `docs/architecture.md` for full layer diagram.
