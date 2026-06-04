#ifndef SVC_SENSOR_H
#define SVC_SENSOR_H

/* ============================================================
 *  svc_sensor.h — Sensor Service
 *  Owns P35 float switch debounce logic and water level
 *  percentage calculation. No GPIO calls here — uses hal_gpio.
 * ============================================================ */

#include <Arduino.h>

typedef enum {
    WATER_EMPTY = 0,
    WATER_MID   = 50,
    WATER_FULL  = 100
} water_level_t;

/* Initialise GPIO pins for both float sensors */
void svc_sensor_init(void);

/* Call every loop(); returns true if the level value changed */
bool svc_sensor_tick(void);

/* Get last stable water level */
water_level_t svc_sensor_get_level(void);

/* Get raw debounced pin states (useful for diagnostics) */
bool svc_sensor_get_high_float(void);
bool svc_sensor_get_low_float(void);

#endif /* SVC_SENSOR_H */
