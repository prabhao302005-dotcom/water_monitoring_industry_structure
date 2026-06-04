/* ============================================================
 *  svc_motor.c — Motor / Relay Control Service
 * ============================================================ */

#include "svc_motor.h"
#include "hal_gpio.h"
#include "hal_uart.h"
#include "drv_blynk.h"
#include "config/sensor_config.h"

static bool         _motor_running = false;
static motor_mode_t _mode          = MOTOR_MODE_AUTO;

void svc_motor_init(void) {
    Serial.begin(115200);
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, RELAY_OFF); /* safe default: OFF */
    _motor_running = false;
    Serial.println("[Motor] Relay initialised — OFF.");
}

void svc_motor_on(const char *reason) {
    if (_motor_running) return;
    digitalWrite(RELAY_PIN, RELAY_ON);
    _motor_running = true;

    Serial.print("[Motor] ON → ");
    Serial.println(reason);

    /* Notify Blynk app */
    drv_blynk_send_motor_label(true);
    drv_blynk_send_motor_cmd(true);
    drv_blynk_send_alert("Motor ON — " + String(reason));
}

void svc_motor_off(const char *reason) {
    if (!_motor_running) return;
    digitalWrite(RELAY_PIN, RELAY_OFF);
    _motor_running = false;

    Serial.print("[Motor] OFF → ");
    Serial.println(reason);

    drv_blynk_send_motor_label(false);
    drv_blynk_send_motor_cmd(false);
    drv_blynk_send_alert("Motor OFF — " + String(reason));
}

bool svc_motor_is_running(void) {
    return _motor_running;
}

motor_mode_t svc_motor_get_mode(void) {
    return _mode;
}

void svc_motor_set_mode(motor_mode_t mode) {
    _mode = mode;
    const char *label = (mode == MOTOR_MODE_AUTO) ? "AUTO" : "MANUAL";
    Serial.print("[Motor] Mode → ");
    Serial.println(label);
    drv_blynk_send_mode(mode == MOTOR_MODE_AUTO);
    drv_blynk_send_alert("Mode changed to " + String(label));
}
