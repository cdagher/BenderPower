# LED1642GW RP2040 Driver

This is a lightweight C++ driver for the STMicroelectronics LED1642GW 16-channel constant-current LED driver. It is designed to run on RP2040-based boards such as the Raspberry Pi Pico, using SPI for communication and GPIO pins for latching and PWM clock control.

## Features

- Control all 16 LED output channels over SPI
- Set individual or global brightness levels
- Accept brightness input as either 16-bit raw values or 0–100 percent
- Enable/disable specific outputs using the switch register
- Configure device settings through the configuration register
- Support for basic open/short/combined error detection
- Optimized for use with 16-bit PWM mode

## Hardware Requirements

- RP2040-based board (e.g. Raspberry Pi Pico)
- LED1642GW chip with external components as recommended by the datasheet
- External square wave signal on PWCLK (e.g. from GPIO or timer peripheral)

## Pin Connections (Example)

| LED1642GW Pin | RP2040 GPIO Example | Function                |
|---------------|---------------------|-------------------------|
| SDI           | GPIO 3              | SPI MOSI (Data In)      |
| CLK           | GPIO 4              | SPI Clock               |
| LE            | GPIO 6              | Latch Enable (manual)   |
| PWCLK         | GPIO 7              | PWM Clock Input         |
| CS            | GPIO 5              | SPI Chip Select (manual)|
| SDO           | Optional            | Serial Data Out         |

## Getting Started

### 1. Include the Library

Copy `led1642gw.h` and `led1642gw.cpp` into your project and include the header in your application code.

### 2. Example Usage

```cpp
#include "led1642gw.h"

LED1642GW driver(spi0, 5, 6, 7); // CS=5, LE=6, PWCLK=7

int main() {
    stdio_init_all();
    driver.begin();

    // Turn on all LEDs at 50% brightness
    driver.setBrightnessPercentAll(50);
    driver.enableAllOutputs();

    // Turn on only LED channel 3 at 75% brightness
    driver.setSingleLEDPercent(3, 75);
}
````

## Public API

### Initialization

```cpp
LED1642GW(spi_inst_t* spi, uint cs_pin, uint le_pin, uint pwclk_pin);
void begin();
```

### Brightness Control

```cpp
void setBrightnessAll(uint16_t brightness);             // Raw 16-bit value
void setBrightnessPercentAll(uint8_t percent);          // 0 to 100 percent
void setSingleLED(uint8_t channel, uint16_t brightness);
void setSingleLEDPercent(uint8_t channel, uint8_t percent);
```

### Output Control

```cpp
void enableAllOutputs();                                // Enable all 16 channels
```

### Configuration

```cpp
void setConfig(uint16_t cfg_value);                     // Load configuration register
void writeConfig();                                     // Latch configuration
```

### Error Detection

```cpp
void startErrorDetection(uint8_t mode);                 // mode: 1=open, 2=short, 3=combined
uint16_t readErrorStatus();                             // Read 16-bit result from SDO
```

## Notes

* This library assumes the LED1642GW is operating in 16-bit PWM mode. If using 12-bit mode, scaling logic must be updated accordingly.
* The PWCLK input must be driven with a continuous square wave. For 16-bit mode, it should be at least 7 MHz to avoid visible flicker.
* LE pulses are required after each data transmission to latch values into the correct internal register.


## Author

Developed by Oliver MacDonald, 2025.