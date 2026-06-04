#ifndef SVC_CONTROLLER_H
#define SVC_CONTROLLER_H

/* ============================================================
 *  svc_controller.h — Top-level Controller Service
 *  Orchestrates sensor readings, motor decisions, alerts,
 *  and periodic Blynk publishes. main.c calls only these.
 *
 *  Dependency graph (no layer skipping):
 *    main.c
 *      └── svc_controller
 *            ├── svc_sensor    (reads P35 float switches)
 *            ├── svc_motor     (controls relay)
 *            ├── svc_alert     (SMS + Blynk alerts)
 *            ├── drv_blynk     (push telemetry to app)
 *            └── drv_gsm       (non-blocking SMS tick)
 * ============================================================ */

#include <Arduino.h>

/* Initialise all sub-services; call once in setup() */
void svc_controller_init(void);

/* Call every loop() — runs all timed tasks non-blocking */
void svc_controller_tick(void);

/* Blynk callbacks — registered during init, called by drv_blynk */
void svc_controller_on_motor_cmd(bool turn_on);
void svc_controller_on_mode_change(bool auto_mode);

#endif /* SVC_CONTROLLER_H */
