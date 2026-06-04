#ifndef SVC_MOTOR_H
#define SVC_MOTOR_H

/* ============================================================
 *  svc_motor.h — Motor / Relay Control Service
 *  Owns relay GPIO and auto/manual mode logic.
 *  Notifies Blynk on every state change.
 * ============================================================ */

#include <Arduino.h>

typedef enum {
    MOTOR_MODE_AUTO   = 0,
    MOTOR_MODE_MANUAL = 1
} motor_mode_t;

/* Initialise relay GPIO */
void svc_motor_init(void);

/* Turn motor ON/OFF with a reason string (for logs/Blynk alert) */
void svc_motor_on(const char *reason);
void svc_motor_off(const char *reason);

/* Get current running state */
bool svc_motor_is_running(void);

/* Get/set operating mode */
motor_mode_t svc_motor_get_mode(void);
void svc_motor_set_mode(motor_mode_t mode);

#endif /* SVC_MOTOR_H */
