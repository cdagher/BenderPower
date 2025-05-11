#include "LED1642GW.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"

LED1642GW::LED1642GW(uint sclk_pin, uint sdata_pin, uint le_pin)
    : _sclk_pin(sclk_pin), _sdata_pin(sdata_pin), _le_pin(le_pin)
{
    _brightness_buffer.fill(0); // All LEDs off by default
}

void LED1642GW::begin() {
    gpio_init(_sclk_pin);
    gpio_set_dir(_sclk_pin, GPIO_OUT);
    gpio_put(_sclk_pin, 0);

    gpio_init(_sdata_pin);
    gpio_set_dir(_sdata_pin, GPIO_OUT);
    gpio_put(_sdata_pin, 0);

    gpio_init(_le_pin);
    gpio_set_dir(_le_pin, GPIO_OUT);
    gpio_put(_le_pin, 0);
}

void LED1642GW::setAllOn() {
    _brightness_buffer.fill(0xFFFF);
    update();
}

void LED1642GW::setAllOff() {
    _brightness_buffer.fill(0x0000);
    update();
}

void LED1642GW::setLedOn(uint8_t index, uint16_t brightness) {
    if (index < 16) {
        _brightness_buffer[index] = brightness;
        update();
    }
}

void LED1642GW::setLedOff(uint8_t index) {
    if (index < 16) {
        _brightness_buffer[index] = 0x0000;
        update();
    }
}

void LED1642GW::update() {
    // Send first 15 brightness values (data latch required)
    for (int i = 15; i > 0; --i) {
        shiftOut16(_brightness_buffer[i]);
        gpio_put(_le_pin, 1);
        for (int j = 0; j < 3; ++j) pulsePin(_sclk_pin); // Data latch: LE high for 3 rising edges
        gpio_put(_le_pin, 0);
    }

    // Send last brightness value (global latch required)
    shiftOut16(_brightness_buffer[0]);
    gpio_put(_le_pin, 1);
    for (int j = 0; j < 5; ++j) pulsePin(_sclk_pin); // Global latch: LE high for 5 rising edges
    gpio_put(_le_pin, 0);

    // Send switch register: all outputs ON (0xFFFF)
    shiftOut16(0xFFFF);
    gpio_put(_le_pin, 1);
    pulsePin(_sclk_pin); // LE high for 1 clock rising edge for switch
    gpio_put(_le_pin, 0);
}

void LED1642GW::shiftOut16(uint16_t value) {
    for (int8_t i = 15; i >= 0; --i) {
        bool bit = (value >> i) & 0x1;
        gpio_put(_sdata_pin, bit);
        pulsePin(_sclk_pin);
    }
}

void LED1642GW::pulsePin(uint pin) {
    gpio_put(pin, 0);
    sleep_us(1); // small delay for timing margin
    gpio_put(pin, 1);
    sleep_us(1);
    gpio_put(pin, 0);
}
