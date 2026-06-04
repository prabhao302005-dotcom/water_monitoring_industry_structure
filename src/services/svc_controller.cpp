/* ============================================================
 *  svc_controller.c — Top-level Controller Service
 *  All timed tasks run here via millis() — no delay() anywhere.
 * ============================================================ */
#ifndef SVC_CONTROLLER_H
#define SVC_CONTROLLER_H
#include <Arduino.h>
#include <WiFi.h>
#include "hal_uart.h"
#include "svc_controller.h"
#include "svc_sensor.h"
#include "svc_motor.h"
#include "svc_alert.h"
#include "drv_blynk.h"
#include "drv_gsm.h"
#include "hal_gpio.h"
#include "hal_uart.h"
#include "config/app_config.h"
#include "config/sensor_config.h"
#include <esp_task_wdt.h>

/* ── Timing ───────────────────────────────────────────────── */
static unsigned long _t_sensor  = 0;
static unsigned long _t_publish = 0;
static unsigned long _t_status  = 0;
static unsigned long _boot_time = 0;

/* ── Heartbeat LED ────────────────────────────────────────── */
static unsigned long _t_led     = 0;
static bool          _led_state = false;

static void _heartbeat(void) {
    unsigned long now = millis();
    unsigned long interval = drv_blynk_is_connected()
                             ? LED_BLINK_CONNECTED
                             : LED_BLINK_NOCONN;
    if (now - _t_led > interval) {
        _t_led = now;
        _led_state = !_led_state;
        hal_gpio_write(LED_PIN, _led_state ? HIGH : LOW);
    }
}

/* ── Sensor + auto-control task ───────────────────────────── */
static void _run_sensor_task(void) {
    bool level_changed = svc_sensor_tick();
    water_level_t level = svc_sensor_get_level();
    bool high_float = svc_sensor_get_high_float();
    bool low_float  = svc_sensor_get_low_float();
    bool motor_on   = svc_motor_is_running();
    bool auto_mode  = (svc_motor_get_mode() == MOTOR_MODE_AUTO);

    /* Serial diagnostic */
    Serial.println("---------------------------");
    Serial.print("HIGH (FULL) float : ");
    Serial.println(high_float ? "UP (water)" : "DOWN (empty)");
    Serial.print("LOW (EMPTY) float : ");
    Serial.println(low_float  ? "UP (water)" : "DOWN (empty)");
    Serial.print("Water level       : ");
    Serial.println((int)level);
    Serial.print("Motor             : ");
    Serial.println(motor_on  ? "RUNNING" : "STOPPED");
    Serial.print("Mode              : ");
    Serial.println(auto_mode ? "AUTO"    : "MANUAL");
    Serial.println("---------------------------");

    if (auto_mode) {
        /* EMPTY: both floats down → start motor */
        if (!low_float && !high_float) {
            svc_motor_on("Tank empty — auto");
            svc_alert_trigger(ALERT_TANK_EMPTY);
        }

        /* FULL: top float up → stop motor */
        if (high_float) {
            svc_motor_off("Tank full — auto");
            svc_alert_trigger(ALERT_TANK_FULL);
        }

        /* MID: low float up, high float down — motor keeps running */
    }

    if (level_changed) {
        const char *lbl = (level == WATER_FULL)  ? "FULL"  :
                          (level == WATER_MID)   ? "MID"   : "EMPTY";
        drv_blynk_send_alert("Level changed to " + String(lbl));
        drv_blynk_send_level((int)level);
    }
}

/* ── Blynk push task ──────────────────────────────────────── */
static void _run_publish_task(void) {
    drv_blynk_send_level((int)svc_sensor_get_level());
    drv_blynk_send_motor_label(svc_motor_is_running());
    drv_blynk_send_mode(svc_motor_get_mode() == MOTOR_MODE_AUTO);
}

/* ── Health status task ───────────────────────────────────── */
static void _run_status_task(void) {
    String st = "UP:" + String((millis() - _boot_time) / 1000) + "s"
              + " | RSSI:" + String(WiFi.RSSI()) + "dBm"
              + " | HEAP:" + String(ESP.getFreeHeap()) + "B";
    drv_blynk_send_status(st);
}

/* ── Blynk callbacks (registered in init) ────────────────── */

void svc_controller_on_motor_cmd(bool turn_on) {
    if (svc_motor_get_mode() != MOTOR_MODE_MANUAL) {
        hal_uart_debug_println("[Ctrl] Motor cmd ignored — AUTO mode");
        drv_blynk_send_alert("Command ignored: switch to MANUAL first");
        return;
    }
    if (turn_on) svc_motor_on("Manual — Blynk app");
    else         svc_motor_off("Manual — Blynk app");
}

void svc_controller_on_mode_change(bool auto_mode) {
    svc_motor_set_mode(auto_mode ? MOTOR_MODE_AUTO : MOTOR_MODE_MANUAL);
}

/* ── Init ─────────────────────────────────────────────────── */

void svc_controller_init(void) {
    _boot_time = millis();

    /* Watchdog */
  /*  esp_task_wdt_config_t wdt = {
        .timeout_ms     = CFG_WDT_TIMEOUT_SEC * 1000U,
        .idle_core_mask = (1 << 0),
        .trigger_panic  = true
    };
    esp_task_wdt_init(&wdt);
    esp_task_wdt_add(NULL);()*/
    esp_task_wdt_init(CFG_WDT_TIMEOUT_SEC, true);
    esp_task_wdt_add(NULL);

    /* LED */
    hal_gpio_init_output(LED_PIN, LOW);

    /* Sensor + motor + alert */
    svc_sensor_init();
    svc_motor_init();
    svc_alert_init();

    /* GSM */
    drv_gsm_init(CFG_GSM_BAUD, GSM_RX_PIN, GSM_TX_PIN,
                 CFG_GSM_INIT_TIMEOUT_MS);

    /* Blynk (also connects WiFi) */
    drv_blynk_init(CFG_BLYNK_AUTH_TOKEN,
                   CFG_WIFI_SSID, CFG_WIFI_PASSWORD,
                   svc_controller_on_motor_cmd,
                   svc_controller_on_mode_change);

    /* Startup SMS */
    drv_gsm_queue_sms(CFG_PHONE_NUMBER,
        "Water Monitor v3.0 ONLINE.\n"
        "IP: " + WiFi.localIP().toString() + "\n"
        "Blynk connected.\n- System Ready");

    drv_blynk_send_alert("System ONLINE — v3.0 ready");
    hal_uart_debug_println("[Ctrl] All services ready.");
}

/* ── Tick ─────────────────────────────────────────────────── */

void svc_controller_tick(void) {
    unsigned long now = millis();

    esp_task_wdt_reset();           /* feed watchdog */

    drv_blynk_tick();               /* Blynk run     */
    drv_gsm_tick();                 /* SMS state machine */
    drv_gsm_passthrough();          /* GSM debug output  */
    _heartbeat();

    if (now - _t_sensor >= CFG_SENSOR_INTERVAL_MS) {
        _t_sensor = now;
        _run_sensor_task();
    }

    if (now - _t_publish >= CFG_PUBLISH_INTERVAL_MS) {
        _t_publish = now;
        _run_publish_task();
    }

    if (now - _t_status >= CFG_STATUS_INTERVAL_MS) {
        _t_status = now;
        _run_status_task();
    }
}
#endif