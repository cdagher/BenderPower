#include "led1642gw.h"
#include "hardware/spi.h"

LED1642GW::LED1642GW(spi_inst_t* spi, uint cs_pin, uint le_pin, uint pwclk_pin)
    : _spi(spi), _cs(cs_pin), _le(le_pin), _pwclk(pwclk_pin) {}

void LED1642GW::begin() {
    gpio_init(_cs); gpio_set_dir(_cs, GPIO_OUT); gpio_put(_cs, 1);
    gpio_init(_le); gpio_set_dir(_le, GPIO_OUT); gpio_put(_le, 0);
    gpio_init(_pwclk); gpio_set_dir(_pwclk, GPIO_OUT); gpio_put(_pwclk, 0);

    spi_init(_spi, 1 * 1000 * 1000);  // 1 MHz
    gpio_set_function(_cs, GPIO_FUNC_SIO);  // CS manually controlled
    gpio_set_function(4, GPIO_FUNC_SPI);   // CLK
    gpio_set_function(3, GPIO_FUNC_SPI);   // MOSI (SDI)
}

void LED1642GW::pulseLE(uint8_t rising_edges) {
    for (int i = 0; i < rising_edges; ++i) {
        gpio_put(_le, 1);
        sleep_us(1);
        gpio_put(_le, 0);
        sleep_us(1);
    }
}

void LED1642GW::writeWord(uint16_t data, uint8_t le_edges) {
    uint8_t tx[2] = { static_cast<uint8_t>(data >> 8), static_cast<uint8_t>(data & 0xFF) };
    gpio_put(_cs, 0);
    spi_write_blocking(_spi, tx, 2);
    gpio_put(_cs, 1);
    pulseLE(le_edges);
}

void LED1642GW::setBrightnessAll(uint16_t brightness) {
    for (int i = 0; i < 15; i++) {
        writeWord(brightness, 3);  // Data latch
    }
    writeWord(brightness, 5);      // Global latch
}

void LED1642GW::setBrightnessPercentAll(uint8_t percent) {
    if (percent > 100) percent = 100;
    uint16_t value = (percent * 65535UL) / 100;
    setBrightnessAll(value);
}

void LED1642GW::enableAllOutputs() {
    writeWord(0xFFFF, 1); // All outputs ON
}

void LED1642GW::setConfig(uint16_t cfg_val) {
    writeWord(cfg_val, 7); // Write config
}

void LED1642GW::writeConfig() {
    pulseLE(7); // Trigger config latch
}

void LED1642GW::startErrorDetection(uint8_t mode) {
    uint8_t edges = 0;
    if (mode == 1) edges = 9;
    else if (mode == 2) edges = 10;
    else if (mode == 3) edges = 11;
    pulseLE(edges);
    sleep_us(1); // Hold ON
    pulseLE(12); // End detection
}

uint16_t LED1642GW::readErrorStatus() {
    gpio_put(_cs, 0);
    uint8_t buf[2];
    spi_read_blocking(_spi, 0x00, buf, 2);
    gpio_put(_cs, 1);
    return (buf[0] << 8) | buf[1];
}

void LED1642GW::setSingleLED(uint8_t channel, uint16_t brightness) {
    if (channel > 15) return;

    uint16_t brightness_words[16] = {0};
    uint16_t switch_mask = (1 << channel);
    brightness_words[channel] = brightness;

    for (int i = 15; i >= 1; i--) {
        writeWord(brightness_words[i], 3);  // Data latch
    }
    writeWord(brightness_words[0], 5);      // Global latch

    writeWord(switch_mask, 1);              // Enable only selected channel
}

void LED1642GW::setSingleLEDPercent(uint8_t channel, uint8_t percent) {
    if (percent > 100) percent = 100;
    uint16_t value = (percent * 65535UL) / 100;
    setSingleLED(channel, value);
}
