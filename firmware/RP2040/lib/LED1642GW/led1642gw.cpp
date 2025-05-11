#include "led1642gw.h"
#include "hardware/spi.h"

LED1642GW::LED1642GW(spi_inst_t* spi_port, uint le)
    : spi(spi_port), pinLE(le) {
    for (int i = 0; i < 16; ++i) brightnessBuffer[i] = 0;
}

void LED1642GW::begin() {
    spi_init(spi, 1000 * 1000); // 1 MHz default

    gpio_init(pinLE);
    gpio_set_dir(pinLE, GPIO_OUT);
    gpio_put(pinLE, 0);
}

void LED1642GW::sendData(const uint8_t* data, size_t len) {
    spi_write_blocking(spi, data, len);
}

void LED1642GW::latch() {
    pulsePin(pinLE);
}

void LED1642GW::writeConfigRegister(uint16_t config) {
    uint8_t buffer[2] = {
        static_cast<uint8_t>((config >> 8) & 0xFF),
        static_cast<uint8_t>(config & 0xFF)
    };
    sendData(buffer, 2);
    // Send 7 CLK pulses with LE high for config write
    gpio_put(pinLE, 1);
    sleep_ms(1);
    gpio_put(pinLE, 0);
}

void LED1642GW::writeBrightnessData(const uint16_t* brightnessData) {
    uint8_t buffer[32];
    for (int i = 0; i < 16; ++i) {
        buffer[i * 2] = (brightnessData[15 - i] >> 8) & 0xFF; // MSB first
        buffer[i * 2 + 1] = brightnessData[15 - i] & 0xFF;
    }
    sendData(buffer, 32);
    // Latch brightness
    for (int i = 0; i < 15; ++i) latch();
    latch(); // Global latch
}

void LED1642GW::turnOnLED(uint8_t index, uint16_t brightness) {
    if (index >= 16) return;
    brightnessBuffer[index] = brightness;
    writeBrightnessData(brightnessBuffer);
}

void LED1642GW::turnOffLED(uint8_t index) {
    if (index >= 16) return;
    brightnessBuffer[index] = 0;
    writeBrightnessData(brightnessBuffer);
}

void LED1642GW::spiWriteByte(uint8_t data) {
    spi_write_blocking(spi, &data, 1);
}

void LED1642GW::pulsePin(uint pin) {
    gpio_put(pin, 1);
    sleep_us(1);
    gpio_put(pin, 0);
    sleep_us(1);
}
