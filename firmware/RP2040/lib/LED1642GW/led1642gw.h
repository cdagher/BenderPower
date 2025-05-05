#pragma once
#include "pico/stdlib.h"

class LED1642GW {
public:
    LED1642GW(spi_inst_t* spi, uint cs_pin, uint le_pin, uint pwclk_pin);
    void begin();

    void setBrightnessAll(uint16_t brightness);
    void setBrightnessPercentAll(uint8_t percent);

    void enableAllOutputs();
    void setConfig(uint16_t cfg_val);
    void writeConfig();

    void setSingleLED(uint8_t channel, uint16_t brightness);
    void setSingleLEDPercent(uint8_t channel, uint8_t percent);

    void startErrorDetection(uint8_t mode); // 1=open, 2=short, 3=combined
    uint16_t readErrorStatus();

private:
    spi_inst_t* _spi;
    uint _cs, _le, _pwclk;

    void writeWord(uint16_t data, uint8_t le_clk_edges);
    void pulseLE(uint8_t rising_edges);
};
