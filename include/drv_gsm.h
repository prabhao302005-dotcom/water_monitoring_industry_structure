#ifndef DRV_GSM_H
#define DRV_GSM_H

/* ============================================================
 *  drv_gsm.h — GSM / SIM800L Driver
 *  Non-blocking SMS via state machine.
 *  Call drv_gsm_init() once, drv_gsm_tick() every loop().
 * ============================================================ */

#include <Arduino.h>

typedef enum {
    GSM_OK          = 0,
    GSM_ERR_TIMEOUT = 1,
    GSM_ERR_BUSY    = 2   /* SMS already queued or sending */
} gsm_status_t;

/* Initialise GSM serial; returns GSM_OK if modem responds */
gsm_status_t drv_gsm_init(uint32_t baud, uint8_t rx_pin, uint8_t tx_pin,
                           uint32_t timeout_ms);

/* Queue one SMS; drops silently if another is already queued/sending */
gsm_status_t drv_gsm_queue_sms(const char *phone, const String &msg);

/* MUST be called every loop() — drives the non-blocking state machine */
void drv_gsm_tick(void);

/* Passthrough: forward any GSM serial bytes to debug UART */
void drv_gsm_passthrough(void);

/* Returns true if the modem initialised successfully */
bool drv_gsm_is_ready(void);

#endif /* DRV_GSM_H */
