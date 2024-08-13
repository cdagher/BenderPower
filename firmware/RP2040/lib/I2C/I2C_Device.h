#ifndef I2C_DEVICE_H
#define I2C_DEVICE_H

#include "../scheduler/task.h"
#include <pico/stdlib.h>

class I2C_Device : public Task {
public:
    I2C_Device(uint8_t address, uint32_t interval)
        : Task(std::bind(&I2C_Device::run, this), interval), address(address) {}

    void run() override;
};

#endif // I2C_DEVICE_H