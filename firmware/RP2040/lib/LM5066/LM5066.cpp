#include "LM5066.h"

#include <stdio.h>
#include <pico/stdlib.h>
#include "pico/binary_info.h"
#include <hardware/i2c.h>

LM5066::LM5066(i2c_inst_t *i2c, uint8_t SDA_PIN, uint8_t SCL_PIN, uint8_t address) {
    _i2c = i2c;
    _SDA_PIN = SDA_PIN;
    _SCL_PIN = SCL_PIN;
    _address = address;
}

bool LM5066::begin(bool pullup, uint clk) {
    i2c_init(_i2c, clk);
    gpio_set_function(_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(_SCL_PIN, GPIO_FUNC_I2C);

    if (pullup) {
        gpio_pull_up(_SDA_PIN);
        gpio_pull_up(_SCL_PIN);
    }

    // Make the I2C pins available to picotool if necessary
    if (_SDA_PIN == PICO_DEFAULT_I2C_SDA_PIN && _SCL_PIN == PICO_DEFAULT_I2C_SCL_PIN) {
        bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C)); }

    return true;
}

void LM5066::OPERATION(bool en) {
    /**
     * @brief Set the operation of the device (enable / disable)
     * 
     * The OPERATION command is a standard PMBus command that controls the MOSFET switch. This command
     * can be used to switch the MOSFET on and off under host control. It is also used to re-enable the 
     * MOSFET after a fault triggered shutdown. Writing an OFF command, followed by an ON command, clears
     * all faults and reenables the device. Writing only an ON after a fault-triggered shutdown does not
     * clear the fault registers or reenable the device. The OPERATION command is issued with the write
     * byte protocol.
     */

    uint8_t cmd = 0x80*en; // 0x80 if en is true, else 0x00

    uint8_t writebuffer[] = {r_OPERATION, cmd};
    uint8_t readbuffer;

    i2c_write_blocking(_i2c, _address, writebuffer, 2, true);
    i2c_read_blocking(_i2c, _address, &readbuffer, 1, false);
}

void LM5066::CLEAR_FAULTS(void) {
    /**
     * @brief Clear all faults on the device
     * 
     * The CLEAR_FAULTS command is a standard PMBus command that resets all stored warning and fault flags
     * and the SMBA signal. If a fault or warning condition still exists when the CLEAR_FAULTS command is
     * issued, the SMBA signal may not clear or re-asserts almost immediately. Issuing a CLEAR_FAULTS command
     * does not cause the MOSFET to switch back on in the event of a fault turnoff; that must be done by
     * issuing an OPERATION command after the fault condition is cleared. This command uses the PMBus send
     * byte protocol.
     */

    uint8_t writebuffer[] = {r_CLEAR_FAULTS};

    i2c_write_blocking(_i2c, _address, writebuffer, 1, true);
}

uint LM5066::CAPABILITY(void) {
    /**
     * @brief Retrieves the capability of the device
     * @returns Device capability byte
     * 
     * The CAPABILITY command is a standard PMBus command that returns information about the PMBus functions
     * supported by the LM5066I. This command is read with the PMBus read byte protocol. Returns 0x0h normally,
     * meaning that the device supports packet error check, 400 Kb/s, supports SMBus alert.
     */

    uint8_t writebuffer[] = {r_CAPABILITY};
    uint8_t readbuffer;

    i2c_write_blocking(_i2c, _address, writebuffer, 1, true);
    i2c_read_blocking(_i2c, _address, &readbuffer, 1, false);

    return readbuffer;
}

uint16_t LM5066::VOUT_UV_WARN_LIMIT(uint16_t limit) {
    /**
     * @brief Set or read the VOUT undervoltage warning limit
     * @param limit The limit to set. 0x0000 disables the limit
     * @returns The limit which was set or retrieved
     * 
     * The VOUT_UV_WARN_LIMIT command is a standard PMBus command that allows configuring or reading the
     * threshold for the VOUT undervoltage warning detection. Reading and writing to this register should
     * use thecoefficients shown in Table 47. Accesses to this command should use the PMBus read or write
     * word protocol. Ifthe measured value of VOUT falls below the value in this register, VOUT UV warn
     * flags are set and the SMBA signal is asserted.
     */

    uint8_t writebuffer[] = {r_VOUT_UV_WARN_LIMIT, (uint8_t)(limit & 0xFF), (uint8_t)(limit >> 8)};
    uint8_t readbuffer[2];

    i2c_write_blocking(_i2c, _address, writebuffer, 3, true);
    i2c_read_blocking(_i2c, _address, readbuffer, 2, false);

    return (readbuffer[1] << 8) | readbuffer[0];
}

uint16_t LM5066::OT_FAULT_LIMIT(uint16_t limit) {
    /**
     * @brief Set or read the overtemperature fault limit temperature
     * @param limit The limit to set. 0x0000 disables the limit
     * @returns The limit which was set or retrieved
     * 
     * The OT_FAULT_LIMIT command is a standard PMBus command that allows configuring or reading the threshold
     * for the overtemperature fault detection. Reading and writing to this register should use the coefficients
     * shown in Table 47. Accesses to this command should use the PMBus read or write word protocol. If the measured
     * temperature exceeds this value, an overtemperature fault is triggered and the MOSFET is switched off, OT
     * FAULT flags set, and the SMBA signal asserted. After the measured temperature falls below the value in this
     * register, the MOSFET may be switched back on with the OPERATION command. A single temperature measurement
     * is an average of 16 round-robin cycles; therefore, the minimum temperature fault detection time is 16 ms.
     */
    
    uint8_t writebuffer[] = {r_OT_FAULT_LIMIT, (uint8_t)(limit & 0xFF), (uint8_t)(limit >> 8)};
    uint8_t readbuffer[2];

    i2c_write_blocking(_i2c, _address, writebuffer, 3, true);
    i2c_read_blocking(_i2c, _address, readbuffer, 2, false);

    return (readbuffer[1] << 8) | readbuffer[0];
}

uint16_t LM5066::OT_WARN_LIMIT(uint16_t limit) {
    /**
     * @brief Set or read the overtemperature warning limit temperature
     * @param limit The limit to set. 0x0000 disables the limit
     * @returns The limit which was set or retrieved
     * 
     * The OT_WARN_LIMIT command is a standard PMBus command that allows configuring or reading the threshold
     * for the over-temperature warning detection. Reading and writing to this register should use the coefficients
     * shown in Table 47. Accesses to this command should use the PMBus read or write word protocol. If the
     * measured temperature exceeds this value, an over-temperature warning is triggered and the OT WARN flags set
     * in the respective registers and the SMBA signal asserted. A single temperature measurement is an average of
     * 16 round-robin cycles; therefore, the minimum temperature warn detection time is 16 ms.
     */
    
    uint8_t writebuffer[] = {r_OT_WARN_LIMIT, (uint8_t)(limit & 0xFF), (uint8_t)(limit >> 8)};
    uint8_t readbuffer[2];

    i2c_write_blocking(_i2c, _address, writebuffer, 3, true);
    i2c_read_blocking(_i2c, _address, readbuffer, 2, false);

    return (readbuffer[1] << 8) | readbuffer[0];
}

uint16_t LM5066::VIN_OV_WARN_LIMIT(uint16_t limit) {
    /**
     * @brief Set or read the VIN overvoltage warning limit
     * @param limit The limit to set. 0x0000 disables the limit
     * @returns The limit which was set or retrieved
     * 
     * The VIN_OV_WARN_LIMIT command is a standard PMBus command that allows configuring or reading the
     * threshold for the VIN overvoltage warning detection. Reading and writing to this register should use the
     * coefficients shown in Table 47. Accesses to this command should use the PMBus read or write word protocol. If
     * the measured value of VIN rises above the value in this register, VIN OV warn flags are set in the respective
     * registers and the SMBA signal is asserted.
     */

    uint8_t writebuffer[] = {r_VIN_OV_WARN_LIMIT, (uint8_t)(limit & 0xFF), (uint8_t)(limit >> 8)};
    uint8_t readbuffer[2];

    i2c_write_blocking(_i2c, _address, writebuffer, 3, true);
    i2c_read_blocking(_i2c, _address, readbuffer, 2, false);

    return (readbuffer[1] << 8) | readbuffer[0];
}

uint16_t LM5066::VIN_UV_WARN_LIMIT(uint16_t limit) {
    /**
     * @brief Set or read the VIN undervoltage warning limit
     * @param limit The limit to set. 0x0000 disables the limit
     * @returns The limit which was set or retrieved
     * 
     * The VIN_UV_WARN_LIMIT command is a standard PMBus command that allows configuring or reading the
     * threshold for the VIN undervoltage warning detection. Reading and writing to this register should use the
     * coefficients shown in Table 47. Accesses to this command should use the PMBus read or write word protocol. If
     * the measured value of VIN falls below the value in this register, VIN UV warn flags are set in the respective
     * register, and the SMBA signal is asserted.
     */
    
    uint8_t writebuffer[] = {r_VIN_UV_WARN_LIMIT, (uint8_t)(limit & 0xFF), (uint8_t)(limit >> 8)};
    uint8_t readbuffer[2];

    i2c_write_blocking(_i2c, _address, writebuffer, 3, true);
    i2c_read_blocking(_i2c, _address, readbuffer, 2, false);

    return (readbuffer[1] << 8) | readbuffer[0];
}

uint LM5066::IIN_OC_WARN_LIMIT(uint) {
    return 0;
}

uint LM5066::STATUS_BYTE(void) {
    return 0;
}

uint LM5066::STATUS_WORD(void) {
    return 0;
}

uint LM5066::STATUS_VOUT(void) {
    return 0;
}

uint LM5066::STATUS_INPUT(void) {
    return 0;
}

uint LM5066::STATUS_TEMPERATURE(void) {
    return 0;
}

uint LM5066::STATUS_CML(void) {
    return 0;
}

uint LM5066::STATUS_OTHER(void) {
    return 0;
}

uint LM5066::STATUS_MFR_SPECIFIC(void) {
    return 0;
}

float LM5066::READ_EIN(void) {
    return 0.0;
}

float LM5066::READ_VIN(void) {
    return 0.0;
}

float LM5066::READ_IIN(void) {
    return 0.0;
}

float LM5066::READ_VOUT(void) {
    return 0.0;
}

float LM5066::READ_TEMPERATURE_1(void) {
    return 0.0;
}

float LM5066::READ_PIN(void) {
    return 0.0;
}

uint LM5066::MFR_ID(void) {
    return 0;
}

uint LM5066::MFR_MODEL(void) {
    return 0;
}

uint LM5066::MFR_REVISION(void) {
    return 0;
}

float LM5066::READ_VAUX(void) {
    return 0.0;
}

float LM5066::MFR_READ_IIN(void) {
    return 0.0;
}

float LM5066::MFR_READ_PIN(void) {
    return 0.0;
}

uint LM5066::MFR_IIN_OC_WARN_LIMIT(uint) {
    return 0;
}

uint LM5066::MFR_PIN_OP_WARN_LIMIT(uint) {
    return 0;
}

float LM5066::READ_PIN_PEAK(void) {
    return 0.0;
}

void LM5066::CLEAR_PIN_PEAK(void) {

}

void LM5066::GATE_MASK(uint) {

}

void LM5066::ALERT_MASK(uint) {

}

void LM5066::DEVICE_SETUP(uint) {

}

uint LM5066::BLOCK_READ(void) {
    return 0;
}

uint LM5066::SAMPLES_FOR_AVG(uint) {
    return 0;
}

float LM5066::READ_AVG_VIN(void) {
    return 0.0;
}

float LM5066::READ_AVG_VOUT(void) {
    return 0.0;
}

float LM5066::READ_AVG_IIN(void) {
    return 0.0;
}

float LM5066::READ_AVG_PIN(void) {
    return 0.0;
}

uint LM5066::BLACK_BOX_READ(void) {
    return 0;
}

uint LM5066::READ_DIAGNOSTIC_WORD(void) {
    return 0;
}

uint* LM5066::AVG_BLOCK_READ(void) {
    return 0;
}
