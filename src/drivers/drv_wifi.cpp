/* ============================================================
 *  drv_wifi.c — WiFi Driver
 * ============================================================ */

#include "drv_wifi.h"
#include "hal_uart.h"
#include <WiFi.h>

static unsigned long _last_reconnect_ms = 0;
#define RECONNECT_COOLDOWN_MS 5000U

wifi_drv_status_t drv_wifi_connect(const char *ssid, const char *pass,
                                   uint32_t timeout_ms) {
    Serial.print("[WiFi] Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);

    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED) {
        if (millis() - start >= timeout_ms) {
            Serial.println("[WiFi] Timeout — GSM-only mode");
            return WIFI_DRV_ERR_TIMEOUT;
        }
        delay(500);
        Serial.print(".");
    }

    Serial.print("\n[WiFi] Connected. IP: ");
    Serial.println(WiFi.localIP().toString().c_str());
    return WIFI_DRV_OK;
}

bool drv_wifi_is_connected(void) {
    return (WiFi.status() == WL_CONNECTED);
}

void drv_wifi_maintain(const char *ssid, const char *pass) {
    if (drv_wifi_is_connected()) return;

    unsigned long now = millis();
    if (now - _last_reconnect_ms < RECONNECT_COOLDOWN_MS) return;
    _last_reconnect_ms = now;

    Serial.println("[WiFi] Disconnected — reconnecting...");
    drv_wifi_connect(ssid, pass, 10000U);
}

String drv_wifi_get_ip(void) {
    return WiFi.localIP().toString();
}

int32_t drv_wifi_get_rssi(void) {
    return WiFi.RSSI();
}
