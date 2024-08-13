#ifndef GPIO_UTILS_H
#define GPIO_UTILS_H

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"


#define PWM_WRAP_MAX 65535


enum PinFunction {
    INPUT,
    INPUT_PULLUP,
    OUTPUT,
    PWM,
    ADC
};

uint16_t PWM_wrap = 65535;

void PWM_set_wrap(uint wrap) {
    if (wrap > PWM_WRAP_MAX) {
        wrap = PWM_WRAP_MAX;
    }
    PWM_wrap = wrap;
}

void GPIO_PWM_Init(uint pin) {
    uint slice = pwm_gpio_to_slice_num(pin);
    uint channel = pwm_gpio_to_channel(pin);

    pwm_set_wrap(slice, PWM_wrap); // set period to 2^16 - 1 cycles
    pwm_set_chan_level(slice, channel, 0);
    pwm_set_enabled(slice, true);
    return;
}

void gpio_init(uint pin, PinFunction fcn) {
    /**
     * @brief Initialize a GPIO pin with a given function
     **/

    switch (fcn) {
        case INPUT:
            gpio_init(pin);
            gpio_set_dir(pin, GPIO_IN);
            break;
        case INPUT_PULLUP:
            gpio_init(pin);
            gpio_set_dir(pin, GPIO_IN);
            gpio_pull_up(pin);
            break;
        case OUTPUT:
            gpio_init(pin);
            gpio_set_dir(pin, GPIO_OUT);
            break;
        case PWM:
            gpio_set_function(pin, GPIO_FUNC_PWM);
            GPIO_PWM_Init(pin);
            pwm_set_gpio_level(pin, 0);
            break;
        case ADC:
            adc_gpio_init(pin);
            break;
    }
}

#endif // GPIO_UTILS_H