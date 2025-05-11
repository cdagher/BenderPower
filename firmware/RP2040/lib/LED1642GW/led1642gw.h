#pragma once
#include "hardware/spi.h"
#include "pico/stdlib.h"

class LED1642GW {
public:
    LED1642GW(spi_inst_t* spi_port, uint le);

    void begin();
    void sendData(const uint8_t* data, size_t len);
    void latch();

    // Utility commands
    void writeConfigRegister(uint16_t config);
    void writeBrightnessData(const uint16_t* brightnessData);  // 16 channels

    // LED control
    void turnOnLED(uint8_t index, uint16_t brightness = 0xFFFF);
    void turnOffLED(uint8_t index);

private:
    spi_inst_t* spi;
    uint pinLE;

    uint16_t brightnessBuffer[16];

    void spiWriteByte(uint8_t data);
    void pulsePin(uint pin);
};
