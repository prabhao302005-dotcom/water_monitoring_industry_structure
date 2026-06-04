#ifndef APP_CONFIG_H
#define APP_CONFIG_H

/* ============================================================
 *  app_config.h — Application-level configuration
 *  Change these values for your deployment.
 *  Do NOT put hardware pin numbers here (→ sensor_config.h).
 * ============================================================ */

/* ── WiFi ──────────────────────────────────────────────────── */
#define CFG_WIFI_SSID          "YourWiFiName"
#define CFG_WIFI_PASSWORD      "YourWiFiPassword"
#define CFG_WIFI_CONNECT_TIMEOUT_MS   10000U   /* 10 s */
#define CFG_WIFI_RECONNECT_INTERVAL_MS 5000U   /* 5 s  */

/* ── Blynk ─────────────────────────────────────────────────── */
#define CFG_BLYNK_TEMPLATE_ID   "TMPLxxxxxxxxxx"
#define CFG_BLYNK_TEMPLATE_NAME "Water Monitor"
#define CFG_BLYNK_AUTH_TOKEN    "your_auth_token"

/* ── GSM / SMS ─────────────────────────────────────────────── */
#define CFG_PHONE_NUMBER        "+91XXXXXXXXXX"
#define CFG_GSM_BAUD            9600U
#define CFG_GSM_INIT_TIMEOUT_MS 8000U
#define CFG_SMS_COOLDOWN_MS     300000U   /* 5 min between same SMS type */

/* ── Task timing ───────────────────────────────────────────── */
#define CFG_SENSOR_INTERVAL_MS  2000U    /* sensor poll rate     */
#define CFG_PUBLISH_INTERVAL_MS 3000U    /* Blynk push rate      */
#define CFG_STATUS_INTERVAL_MS  15000U   /* health status push   */

/* ── Watchdog ──────────────────────────────────────────────── */
#define CFG_WDT_TIMEOUT_SEC     30U

#endif /* APP_CONFIG_H */
