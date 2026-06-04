/* ============================================================
 *  hal_uart.c — UART Hardware Abstraction Layer
 * ============================================================ */

#include "hal_uart.h"
#include <Arduino.h>

HardwareSerial _gsm_serial(2);

//extern HardwareSerial _gsm_serial; /* UART2 for SIM800L */

/* ── Debug UART ─────────────────────────────────────────── */

void hal_uart_debug_init(uint32_t baud) {
    Serial.begin(baud);
}

void hal_uart_debug_print(const char *msg) {
    Serial.print(msg);
}

void hal_uart_debug_println(const char *msg) {
    Serial.println(msg);
}

/* ── GSM UART ───────────────────────────────────────────── */

void hal_uart_gsm_init(uint32_t baud, uint8_t rx_pin, uint8_t tx_pin) {
    _gsm_serial.begin(baud, SERIAL_8N1, rx_pin, tx_pin);
}

void hal_uart_gsm_print(const char *msg) {
    _gsm_serial.print(msg);
}

void hal_uart_gsm_println(const char *msg) {
    _gsm_serial.println(msg);
}

void hal_uart_gsm_write_byte(uint8_t b) {
    _gsm_serial.write(b);
}

bool hal_uart_gsm_available(void) {
    return _gsm_serial.available() > 0;
}

uint8_t hal_uart_gsm_read(void) {
    return _gsm_serial.read();
}

String hal_uart_gsm_read_string(void) {
    return _gsm_serial.readString();
}
