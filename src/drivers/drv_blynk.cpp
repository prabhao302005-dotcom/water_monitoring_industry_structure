/* ============================================================
 *  drv_blynk.c — Blynk IoT Driver
 * ============================================================ */

/* Blynk config MUST be defined before this include */
#include "config/app_config.h"
#define BLYNK_TEMPLATE_ID    CFG_BLYNK_TEMPLATE_ID
#define BLYNK_TEMPLATE_NAME  CFG_BLYNK_TEMPLATE_NAME
#define BLYNK_AUTH_TOKEN     CFG_BLYNK_AUTH_TOKEN

#include <BlynkSimpleEsp32.h>
#include "drv_blynk.h"
#include "config/sensor_config.h"
#include "hal_uart.h"

/* ── Private callbacks (set by init) ─────────────────────── */
static blynk_motor_cmd_cb_t _motor_cb = NULL;
static blynk_mode_cb_t      _mode_cb  = NULL;

/* ── BLYNK_WRITE handlers ───────────────────────────────────
 *  These are global functions required by the Blynk library.
 *  They delegate immediately to the registered callbacks so
 *  all business logic stays in the service layer.
 * ──────────────────────────────────────────────────────────  */

BLYNK_WRITE(V2) {   /* Motor command button from app */
    if (_motor_cb != NULL) {
        _motor_cb(param.asInt() == 1);
    }
}

BLYNK_WRITE(V3) {   /* AUTO/MANUAL mode switch from app */
    if (_mode_cb != NULL) {
        _mode_cb(param.asInt() == 1);   /* 1 = AUTO, 0 = MANUAL */
    }
}

/* ── Public API ───────────────────────────────────────────── */

void drv_blynk_init(const char *auth,
                    const char *ssid, const char *pass,
                    blynk_motor_cmd_cb_t motor_cb,
                    blynk_mode_cb_t      mode_cb) {
    _motor_cb = motor_cb;
    _mode_cb  = mode_cb;

    Serial.println("[Blynk] Connecting...");
    Blynk.begin(auth, ssid, pass);
    Serial.println("[Blynk] Connected.");
}

void drv_blynk_tick(void) {
    Blynk.run();
}

bool drv_blynk_is_connected(void) {
    return Blynk.connected();
}

void drv_blynk_send_level(int level_pct) {
    if (!Blynk.connected()) return;
    Blynk.virtualWrite(VPIN_LEVEL, level_pct);
}

void drv_blynk_send_motor_label(bool motor_on) {
    if (!Blynk.connected()) return;
    Blynk.virtualWrite(VPIN_MOTOR_LABEL, motor_on ? "MOTOR ON" : "Motor OFF");
}

void drv_blynk_send_mode(bool auto_mode) {
    if (!Blynk.connected()) return;
    Blynk.virtualWrite(VPIN_MODE, auto_mode ? 1 : 0);
}

void drv_blynk_send_motor_cmd(bool motor_on) {
    if (!Blynk.connected()) return;
    Blynk.virtualWrite(VPIN_MOTOR_CMD, motor_on ? 1 : 0);
}

void drv_blynk_send_alert(const String &msg) {
    if (!Blynk.connected()) return;
    String stamped = "[" + String(millis() / 1000) + "s] " + msg + "\n";
    Blynk.virtualWrite(VPIN_ALERT, stamped);
}

void drv_blynk_send_status(const String &status) {
    if (!Blynk.connected()) return;
    Blynk.virtualWrite(VPIN_STATUS, status);
}
