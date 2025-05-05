#include <cstdint>
#include <hardware/i2c.h>

#ifndef LM5066I_H
#define LM5066I_H

class LM5066I {
public:
    // Constructor
    LM5066I(
        i2c_inst_t* i2c,
        uint8_t address,
        float32_t voltage_lsb = VOLTAGE_LSB,
        float32_t current_lsb = CURRENT_LSB,
        float32_t power_lsb = POWER_LSB
    );

    // Initialize I2C hardware (user provides GPIO setup separately)
    void begin();

    // General PMBus commands
    void setOperation(bool enable); // Set operation mode (OPERATION_STATUS)
    void clearFaults(); // Clear all faults
    uint8_t capability(); // Get device capabilities

    // Some example register-specific methods
    float readVoltage();      // Reads VOUT or VDD depending on the register
    float readCurrent();      // Reads sense current
    float readPower();        // Reads power usage

private:
    i2c_inst_t* _i2c;
    uint8_t _address;

    // Read a single byte from a register
    uint8_t _readRegister(uint8_t reg);

    // Read a 2-byte word (most registers are 16-bit)
    uint16_t _readWord(uint8_t reg);

    // Write a single byte to a register
    void _writeRegister(uint8_t reg, uint8_t value);

    // Write a 2-byte word to a register
    void _writeWord(uint8_t reg, uint16_t value);

    // Device-specific constants
    float _CURRENT_LSB;
    float _VOLTAGE_LSB;
    float _POWER_LSB;

    // Supported PMBus commands (from datasheet)
    enum Command : uint8_t {
        OPERATION_STATUS    = 0X01,
        CLEAR_FAULTS        = 0x03,
        CAPABILITY          = 0x19,
        VOUT_UV_WARN_LIM    = 0X43,
        OT_FAULT_LIMIT      = 0X4F,
        OT_WARN_LIMIT       = 0X51,
        VIN_OV_WARN_LIM     = 0X57,
        VIN_UV_WARN_LIM     = 0X58,
        IIN_OC_WARN_LIM     = 0X5D,
        STATUS_BYTE         = 0x78,
        STATUS_WORD         = 0x79,
        STATUS_VOUT         = 0x7A,
        STATUS_INPUT        = 0x7C,
        STATUS_TEMP         = 0x7D,
        STATUS_CML          = 0x7E,
        STATUS_OTHER        = 0x7F,
        STATUS_MFR_SPEC     = 0x80,
        READ_EIN            = 0x86,
        READ_VIN            = 0x88,
        READ_IIN            = 0x89,
        READ_VOUT           = 0x8B,
        READ_TEMPERATURE_1  = 0X8D,
        READ_PIN            = 0x97,
        READ_POUT           = 0x98,
        MFR_ID              = 0x99,
        MFR_MODEL           = 0x9A,
        MFR_REVISION        = 0x9B,
        READ_VAUX           = 0xD0,
        MFR_READ_IIN        = 0xD1,
        MFR_READ_PIN        = 0xD2,
        MFR_IIN_OC_WARN_LIM = 0xD3,
        MFR_PIN_OP_WARN_LIM = 0xD4,
        READ_PIN_PEAK       = 0xD5,
        CLEAR_PIN_PEAK      = 0xD6,
        GATE_MASK           = 0xD7,
        ALERT_MASK          = 0xD8,
        DEVICE_SETUP        = 0xD9,
        BLOCK_READ          = 0xDA,
        SAMPLES_FOR_AVG     = 0xDB,
        READ_AVG_VIN        = 0xDC,
        READ_AVG_VOUT       = 0xDD,
        READ_AVG_IIN        = 0xDE,
        READ_AVG_PIN        = 0xDF,
        BLACK_BOX_READ      = 0xE0,
        DIAGNOSTIC_READ     = 0xE1,
        AVG_BLOCK_READ      = 0xE2,
    };
};

#endif // LM5066I_H
