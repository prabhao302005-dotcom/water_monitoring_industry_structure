#ifndef DRV_WIFI_H
#define DRV_WIFI_H

/* ============================================================
 *  drv_wifi.h — WiFi Driver
 *  Owns connection lifecycle; upper layers call drv_wifi_*
 *  and never touch WiFi.h directly.
 * ============================================================ */

#include <Arduino.h>

typedef enum {
    WIFI_DRV_OK          = 0,
    WIFI_DRV_ERR_TIMEOUT = 1,
    WIFI_DRV_ERR_LOST    = 2
} wifi_drv_status_t;

/* Connect; blocks up to timeout_ms. Returns WIFI_DRV_OK on success. */
wifi_drv_status_t drv_wifi_connect(const char *ssid, const char *pass,
                                   uint32_t timeout_ms);

/* Returns true if currently connected */
bool drv_wifi_is_connected(void);

/* Call in loop to auto-reconnect when connection drops */
void drv_wifi_maintain(const char *ssid, const char *pass);

/* Get local IP as string (caller must not free the pointer) */
String drv_wifi_get_ip(void);

/* Get RSSI in dBm */
int32_t drv_wifi_get_rssi(void);

#endif /* DRV_WIFI_H */
