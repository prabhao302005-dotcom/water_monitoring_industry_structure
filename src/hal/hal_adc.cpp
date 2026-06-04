/* ============================================================
 *  hal_adc.c — ADC Hardware Abstraction Layer
 * ============================================================ */

#include "hal_adc.h"

static uint8_t _adc_bits = 12;

void hal_adc_init(uint8_t bits) {
    _adc_bits = bits;
    analogReadResolution(bits);
}

uint16_t hal_adc_read_raw(uint8_t pin) {
    return (uint16_t)analogRead(pin);
}

float hal_adc_read_mv(uint8_t pin) {
    uint16_t raw  = hal_adc_read_raw(pin);
    uint16_t full = (1U << _adc_bits) - 1U;   /* e.g. 4095 for 12-bit */
    return (raw / (float)full) * 3300.0f;      /* 3300 mV reference    */
}
