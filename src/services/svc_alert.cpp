/* ============================================================
 *  svc_alert.c — Alert / Notification Service
 *  SMS cooldown prevents flooding the user's phone.
 *  One SMS per event type; flag resets when the opposite
 *  event occurs (empty→full resets empty flag, and vice versa).
 * ============================================================ */

#include "svc_alert.h"
#include "drv_gsm.h"
#include "drv_blynk.h"
#include "hal_uart.h"
#include "config/app_config.h"

static bool _sms_sent_empty = false;
static bool _sms_sent_full  = false;

void svc_alert_init(void) {
    _sms_sent_empty = false;
    _sms_sent_full  = false;
}

void svc_alert_trigger(alert_type_t type) {
    switch (type) {

        case ALERT_TANK_EMPTY:
            drv_blynk_send_alert("TANK EMPTY — Motor started automatically");
            if (!_sms_sent_empty) {
                _sms_sent_empty = true;
                _sms_sent_full  = false;  /* allow full SMS next time */
                drv_gsm_queue_sms(
                    CFG_PHONE_NUMBER,
                    "WATER TANK ALERT!\n"
                    "Tank is EMPTY.\n"
                    "Motor started automatically.\n"
                    "- Water Monitor v3"
                );
            }
            break;

        case ALERT_TANK_FULL:
            drv_blynk_send_alert("TANK FULL — Motor stopped automatically");
            if (!_sms_sent_full) {
                _sms_sent_full  = true;
                _sms_sent_empty = false;  /* allow empty SMS next time */
                drv_gsm_queue_sms(
                    CFG_PHONE_NUMBER,
                    "WATER TANK INFO:\n"
                    "Tank is FULL.\n"
                    "Motor stopped automatically.\n"
                    "- Water Monitor v3"
                );
            }
            break;
    }
}

void svc_alert_reset(void) {
    _sms_sent_empty = false;
    _sms_sent_full  = false;
}
