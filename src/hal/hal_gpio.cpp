/* ============================================================
 *  hal_gpio.c — GPIO Hardware Abstraction Layer
 * ============================================================ */

#include "hal_gpio.h"

void hal_gpio_init_input(uint8_t pin) {
    pinMode(pin, INPUT);
}

void hal_gpio_init_output(uint8_t pin, uint8_t initial_state) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, initial_state);
}

uint8_t hal_gpio_read(uint8_t pin) {
    return digitalRead(pin);
}

void hal_gpio_write(uint8_t pin, uint8_t state) {
    digitalWrite(pin, state);
}

void hal_gpio_toggle(uint8_t pin) {
    digitalWrite(pin, !digitalRead(pin));
}
