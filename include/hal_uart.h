#ifndef HAL_UART_H
#define HAL_UART_H

/* ============================================================
 *  hal_uart.h — UART Hardware Abstraction Layer
 *  Abstracts Arduino HardwareSerial for debug port and
 *  the GSM serial port separately.
 * ============================================================ */

#include <Arduino.h>

/* ── Debug UART (USB Serial) ─────────────────────────────── */
void hal_uart_debug_init(uint32_t baud);
void hal_uart_debug_print(const char *msg);
void hal_uart_debug_println(const char *msg);

/* ── GSM UART (HardwareSerial 2) ────────────────────────── */
void    hal_uart_gsm_init(uint32_t baud, uint8_t rx_pin, uint8_t tx_pin);
void    hal_uart_gsm_print(const char *msg);
void    hal_uart_gsm_println(const char *msg);
void    hal_uart_gsm_write_byte(uint8_t b);
bool    hal_uart_gsm_available(void);
uint8_t hal_uart_gsm_read(void);
String  hal_uart_gsm_read_string(void);

#endif /* HAL_UART_H */
