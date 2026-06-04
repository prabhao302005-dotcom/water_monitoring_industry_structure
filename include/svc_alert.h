#ifndef SVC_ALERT_H
#define SVC_ALERT_H

/* ============================================================
 *  svc_alert.h — Alert / Notification Service
 *  Owns SMS cooldown logic and Blynk alert routing.
 *  Neither the sensor nor motor service sends SMS directly.
 * ============================================================ */

#include <Arduino.h>

typedef enum {
    ALERT_TANK_EMPTY = 0,
    ALERT_TANK_FULL  = 1
} alert_type_t;

/* Initialise alert state */
void svc_alert_init(void);

/* Trigger an alert; handles SMS cooldown internally */
void svc_alert_trigger(alert_type_t type);

/* Reset all cooldown flags (e.g. on system restart) */
void svc_alert_reset(void);

#endif /* SVC_ALERT_H */
