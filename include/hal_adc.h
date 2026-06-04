#ifndef HAL_ADC_H
#define HAL_ADC_H

/* ============================================================
 *  hal_adc.h — ADC Hardware Abstraction Layer
 *  Reserved for future analog sensor expansion.
 *  Current design uses P35 digital float switches only.
 *
 *  Future use example: 4–20 mA level transmitter via ADC.
 * ============================================================ */

#include <Arduino.h>

/* Initialise ADC resolution (default ESP32: 12-bit = 0-4095) */
void hal_adc_init(uint8_t bits);

/* Read raw ADC count from a pin (0 to (2^bits - 1)) */
uint16_t hal_adc_read_raw(uint8_t pin);

/* Read ADC and scale to millivolts (assumes 3.3 V reference) */
float hal_adc_read_mv(uint8_t pin);

#endif /* HAL_ADC_H */
