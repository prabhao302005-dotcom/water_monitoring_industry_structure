#ifndef SENSOR_CONFIG_H
#define SENSOR_CONFIG_H

/* ============================================================
 *  sensor_config.h — Hardware pin map & sensor constants
 *  All GPIO numbers live here and ONLY here.
 * ============================================================ */

/* ── P35 Float Switch GPIO pins ────────────────────────────────
 *  Wiring:
 *    Float wire 1 → GPIO pin below
 *    Float wire 2 → 3.3 V
 *    10 kΩ pull-down resistor from each GPIO to GND  (REQUIRED —
 *    GPIO34 and GPIO35 have no internal pull-down on ESP32)
 *
 *  P35 behaviour (normally-open):
 *    Float UP   (water present) → switch closes → pin HIGH
 *    Float DOWN (no water)      → switch opens  → pin LOW
 *
 *  If your P35 is NC type, set SENSOR_ACTIVE_STATE to LOW.
 * ─────────────────────────────────────────────────────────── */
#define SENSOR_PIN_HIGH      34   /* TOP float    — FULL level  */
#define SENSOR_PIN_LOW       35   /* BOTTOM float — EMPTY level */
#define SENSOR_ACTIVE_STATE  HIGH /* Change to LOW for NC type  */
#define SENSOR_DEBOUNCE_MS   500U /* float must be stable 500 ms */

/* Water level percentage values */
#define LEVEL_FULL           100
#define LEVEL_MID             50
#define LEVEL_EMPTY            0

/* ── Relay ─────────────────────────────────────────────────── */
#define RELAY_PIN            26
#define RELAY_ON             LOW    /* active-low optocoupler   */
#define RELAY_OFF            HIGH

/* ── GSM serial ────────────────────────────────────────────── */
#define GSM_RX_PIN           16    /* ESP32 RX2 ← SIM800L TX   */
#define GSM_TX_PIN           17    /* ESP32 TX2 → SIM800L RX   */

/* ── Status LED ────────────────────────────────────────────── */
#define LED_PIN               2    /* onboard LED               */
#define LED_BLINK_CONNECTED   1000U /* ms — slow blink when OK  */
#define LED_BLINK_NOCONN      200U  /* ms — fast blink when err */

/* ── Blynk virtual pins ────────────────────────────────────── */
#define VPIN_LEVEL           V0   /* Gauge 0-100 → app         */
#define VPIN_MOTOR_LABEL     V1   /* Label "ON"/"OFF" → app    */
#define VPIN_MOTOR_CMD       V2   /* Button 0/1 ← app          */
#define VPIN_MODE            V3   /* Switch AUTO/MANUAL ← app  */
#define VPIN_ALERT           V4   /* Terminal log → app        */
#define VPIN_STATUS          V5   /* Health string → app       */

#endif /* SENSOR_CONFIG_H */
