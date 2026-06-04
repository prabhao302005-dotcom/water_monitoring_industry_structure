/* ============================================================
 *  drv_gsm.c — GSM / SIM800L Driver
 *  Non-blocking SMS send via state machine.
 *  Original v2.0 code had delay(4000) inside sendSMS() — wrong.
 *  This version never blocks the main loop.
 * ============================================================ */

#include "drv_gsm.h"
#include "hal_uart.h"

/* ── Private state ────────────────────────────────────────── */
typedef enum {
    SMS_STATE_IDLE   = 0,
    SMS_STATE_HEADER = 1,   /* sent AT+CMGS, waiting 500 ms */
    SMS_STATE_BODY   = 2,   /* sent body, waiting 200 ms    */
    SMS_STATE_CTRL_Z = 3,   /* sent Ctrl+Z, waiting 4000 ms */
} sms_state_t;

static bool         _gsm_ready    = false;
static bool         _sms_queued   = false;
static bool         _sms_sending  = false;
static sms_state_t  _sms_state    = SMS_STATE_IDLE;
static String       _queued_msg   = "";
static String       _queued_phone = "";
static unsigned long _step_start  = 0;

/* ── Init ─────────────────────────────────────────────────── */

gsm_status_t drv_gsm_init(uint32_t baud, uint8_t rx_pin, uint8_t tx_pin,
                           uint32_t timeout_ms) {
    Serial.println("[GSM] Initializing SIM800L...");
    hal_uart_gsm_init(baud, rx_pin, tx_pin);

    unsigned long start = millis();
    while (millis() - start < timeout_ms) {
        hal_uart_gsm_println("AT");
        delay(300);
        if (hal_uart_gsm_available()) {
            String resp = hal_uart_gsm_read_string();
            if (resp.indexOf("OK") >= 0) {
                hal_uart_gsm_println("AT+CMGF=1");   delay(300);
                hal_uart_gsm_println("AT+CSCS=\"GSM\""); delay(300);
                Serial.println("[GSM] Ready.");
                _gsm_ready = true;
                return GSM_OK;
            }
        }
    }

    Serial.println("[GSM] No response — GSM disabled.");
    _gsm_ready = false;
    return GSM_ERR_TIMEOUT;
}

bool drv_gsm_is_ready(void) {
    return _gsm_ready;
}

/* ── Queue SMS ────────────────────────────────────────────── */

gsm_status_t drv_gsm_queue_sms(const char *phone, const String &msg) {
    if (!_gsm_ready)               return GSM_ERR_BUSY;
    if (_sms_queued || _sms_sending) return GSM_ERR_BUSY;

    _queued_phone = String(phone);
    _queued_msg   = msg;
    _sms_queued   = true;

    Serial.print("[SMS] Queued → ");
    Serial.println(msg.substring(0, 30).c_str());
    return GSM_OK;
}

/* ── Non-blocking tick (call every loop) ─────────────────── */

void drv_gsm_tick(void) {
    if (!_gsm_ready) return;
    if (!_sms_queued && !_sms_sending) return;

    unsigned long now = millis();

    /* Start sending if queued and idle */
    if (_sms_queued && !_sms_sending) {
        _sms_queued  = false;
        _sms_sending = true;
        _sms_state   = SMS_STATE_IDLE;
    }

    if (!_sms_sending) return;

    switch (_sms_state) {

        case SMS_STATE_IDLE:
            /* Send AT+CMGS header */
            hal_uart_gsm_print("AT+CMGS=\"");
            hal_uart_gsm_print(_queued_phone.c_str());
            hal_uart_gsm_println("\"");
            _step_start = now;
            _sms_state  = SMS_STATE_HEADER;
            break;

        case SMS_STATE_HEADER:
            /* Wait 500 ms for modem prompt '>' */
            if (now - _step_start >= 500UL) {
                hal_uart_gsm_print(_queued_msg.c_str());
                _step_start = now;
                _sms_state  = SMS_STATE_BODY;
            }
            break;

        case SMS_STATE_BODY:
            /* Wait 200 ms then send Ctrl+Z to commit */
            if (now - _step_start >= 200UL) {
                hal_uart_gsm_write_byte(26);   /* ASCII 26 = Ctrl+Z */
                _step_start = now;
                _sms_state  = SMS_STATE_CTRL_Z;
            }
            break;

        case SMS_STATE_CTRL_Z:
            /* Wait 4 s for +CMGS confirmation */
            if (now - _step_start >= 4000UL) {
                Serial.println("[SMS] Sent.");
                _sms_sending = false;
                _sms_state   = SMS_STATE_IDLE;
            }
            break;
    }
}

/* ── Debug passthrough ────────────────────────────────────── */

void drv_gsm_passthrough(void) {
    while (hal_uart_gsm_available()) {
        Serial.write(hal_uart_gsm_read());
    }
}
