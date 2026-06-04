/* ============================================================
 *  main.c — Entry point
 *
 *  setup() and loop() contain ZERO business logic.
 *  All logic lives in svc_controller (and its dependencies).
 *  If you find yourself adding an if-statement here, it belongs
 *  in a service instead.
 * ============================================================ */
#include <WiFi.h>
#include "hal_uart.h"
#include "svc_controller.h"

void setup() {
    Serial.begin(115200);
    Serial.println("\n============================================");
    Serial.println("  Water Level Monitor — v3.0");
    Serial.println("  P35 Float Switch + Blynk IoT");
    Serial.println("============================================\n");

    svc_controller_init();
}

void loop() {
    svc_controller_tick();
}
