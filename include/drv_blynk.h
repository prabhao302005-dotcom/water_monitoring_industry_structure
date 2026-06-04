#ifndef DRV_BLYNK_H
#define DRV_BLYNK_H

/* ============================================================
 *  drv_blynk.h — Blynk IoT Driver
 *  Owns all Blynk.virtualWrite / BLYNK_WRITE callbacks.
 *  Upper service layer calls drv_blynk_* functions.
 *  Never call Blynk.* directly outside this driver.
 * ============================================================ */

#include <Arduino.h>

/* Callback type — service layer registers these */
typedef void (*blynk_motor_cmd_cb_t)(bool turn_on);
typedef void (*blynk_mode_cb_t)(bool auto_mode);

/* Initialise Blynk (also connects WiFi internally via Blynk.begin) */
void drv_blynk_init(const char *auth,
                    const char *ssid, const char *pass,
                    blynk_motor_cmd_cb_t motor_cb,
                    blynk_mode_cb_t      mode_cb);

/* Must be called every loop() */
void drv_blynk_tick(void);

/* Returns true if connected to Blynk cloud */
bool drv_blynk_is_connected(void);

/* Push water level gauge (0-100) */
void drv_blynk_send_level(int level_pct);

/* Push motor status label */
void drv_blynk_send_motor_label(bool motor_on);

/* Sync the mode switch widget to current state */
void drv_blynk_send_mode(bool auto_mode);

/* Sync the motor command button widget */
void drv_blynk_send_motor_cmd(bool motor_on);

/* Push a timestamped alert line to the Terminal widget */
void drv_blynk_send_alert(const String &msg);

/* Push health status string */
void drv_blynk_send_status(const String &status);

#endif /* DRV_BLYNK_H */
