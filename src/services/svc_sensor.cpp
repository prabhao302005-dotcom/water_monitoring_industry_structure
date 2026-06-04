/* ============================================================
 *  svc_sensor.c — Sensor Service
 *  Debounces P35 float switches and maps raw states to a
 *  water_level_t enum. No direct Arduino API calls — uses HAL.
 * ============================================================ */

#include "svc_sensor.h"
#include "hal_gpio.h"
#include "hal_uart.h"
#include "config/sensor_config.h"

/* ── Private debounce state ───────────────────────────────── */
static bool   _stable_high     = false;
static bool   _stable_low      = false;
static bool   _last_raw_high   = false;
static bool   _last_raw_low    = false;
static unsigned long _high_stable_ms = 0;
static unsigned long _low_stable_ms  = 0;

static water_level_t _current_level = WATER_EMPTY;

/* ── Init ─────────────────────────────────────────────────── */

void svc_sensor_init(void) {
    /* External 10 kΩ pull-down required on GPIO34/35 */
    pinMode(SENSOR_PIN_HIGH, INPUT);
    pinMode(SENSOR_PIN_LOW, INPUT);
    Serial.println("[Sensor] P35 float switches initialised.");
}

/* ── Internal debounce ────────────────────────────────────── */

static void _debounce_pin(uint8_t pin,
                          bool *last_raw,
                          unsigned long *stable_ms,
                          bool *stable_out) {
    bool raw = (digitalRead(pin) == SENSOR_ACTIVE_STATE);
    unsigned long now = millis();

    if (raw != *last_raw) {
        *stable_ms = now;       /* reset timer on any change */
        *last_raw  = raw;
    }

    if (now - *stable_ms >= SENSOR_DEBOUNCE_MS) {
        *stable_out = raw;      /* accept reading after stable period */
    }
}

/* ── Tick (call every loop) ───────────────────────────────── */

bool svc_sensor_tick(void) {
    _debounce_pin(SENSOR_PIN_HIGH,
                  &_last_raw_high, &_high_stable_ms, &_stable_high);
    _debounce_pin(SENSOR_PIN_LOW,
                  &_last_raw_low,  &_low_stable_ms,  &_stable_low);

    water_level_t new_level;
    if (_stable_high)       new_level = WATER_FULL;
    else if (_stable_low)   new_level = WATER_MID;
    else                    new_level = WATER_EMPTY;

    bool changed = (new_level != _current_level);
    _current_level = new_level;
    return changed;
}

/* ── Accessors ────────────────────────────────────────────── */

water_level_t svc_sensor_get_level(void) {
    return _current_level;
}

bool svc_sensor_get_high_float(void) {
    return _stable_high;
}

bool svc_sensor_get_low_float(void) {
    return _stable_low;
}
