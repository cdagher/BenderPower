#ifndef LED1642GW_H
#define LED1642GW_H

#include "pico/stdlib.h"
#include <cstdint>
#include <array>

class LED1642GW {
public:
    // Constructor: accepts GPIO pin numbers for SCLK, SDATA, and LE
    LED1642GW(uint sclk_pin, uint sdata_pin, uint le_pin);

    // Initialize the GPIO pins and set them as outputs
    void begin();

    // Set all LEDs ON (max brightness)
    void setAllOn();

    // Set all LEDs OFF
    void setAllOff();

    // Set a single LED (0-15) ON (max brightness)
    void setLedOn(uint8_t index, uint16_t brightness = 0xFFFF);

    // Set a single LED (0-15) OFF
    void setLedOff(uint8_t index);

    // Send the current brightness buffer to the LED1642GW
    void update();

private:
    // GPIO pin numbers
    uint _sclk_pin;
    uint _sdata_pin;
    uint _le_pin;

    // 16 channels x 16 bits = 256 bits buffer
    std::array<uint16_t, 16> _brightness_buffer;

    // Shift a 16-bit value MSB-first
    void shiftOut16(uint16_t value);

    // Pulse a pin HIGH then LOW
    void pulsePin(uint pin);
};

#endif // LED1642GW_H