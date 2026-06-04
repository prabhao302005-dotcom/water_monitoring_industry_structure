#ifndef HAL_GPIO_H
#define HAL_GPIO_H

/* ============================================================
 *  hal_gpio.h — GPIO Hardware Abstraction Layer
 *  Wraps Arduino digitalRead/digitalWrite so upper layers
 *  never touch the Arduino API directly.
 * ============================================================ */

#include <Arduino.h>

/* Initialise a pin as input (no internal pull — use external) */
void hal_gpio_init_input(uint8_t pin);

/* Initialise a pin as output and set initial state */
void hal_gpio_init_output(uint8_t pin, uint8_t initial_state);

/* Read a digital pin; returns HIGH or LOW */
uint8_t hal_gpio_read(uint8_t pin);

/* Write a digital pin */
void hal_gpio_write(uint8_t pin, uint8_t state);

/* Toggle a digital pin */
void hal_gpio_toggle(uint8_t pin);

#endif /* HAL_GPIO_H */
