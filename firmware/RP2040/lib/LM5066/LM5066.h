#ifndef LM5066_h
#define LM5066_h

#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/i2c.h>

enum registers {
    r_OPERATION             = 0x01, // Retrieves or stores the operation status (R/W)
    r_CLEAR_FAULTS          = 0x03, // Clears the status registers and re-arms the black box registers for updating (send byte)
    r_CAPABILITY            = 0x19, // Retrieves the device capability (R)
    r_VOUT_UV_WARN_LIMIT    = 0x43, // Retrieves or stores output undervoltage warn limit threshold (R/W)
    r_OT_FAULT_LIMIT        = 0x4f, // Retrieves or stores over temperature fault limit threshold (R/W)
    r_OT_WARN_LIMIT         = 0x51, // Retrieves or stores over temperature warn limit threshold (R/W)
    r_VIN_OV_WARN_LIMIT     = 0x57, // Retrieves or stores input overvoltage warn limit threshold (R/W)
    r_VIN_UV_WARN_LIMIT     = 0x58, // Retrieves or stores input undervoltage warn limit threshold (R/W)
    r_IIN_OC_WARN_LIMIT     = 0x5d, // Retrieves or stores input current warn limit threshold (mirror at D3h) (R/W)
    r_STATUS_BYTE           = 0x78, // Retrieves information about the part's operating status (R)
    r_STATUS_WORD           = 0x79, // Retrieves information about the part's operating status (R)
    r_STATUS_VOUT           = 0x7a, // Retrieves information about output voltage status (R)
    r_STATUS_INPUT          = 0x7c, // Retrieves information about input status (R)
    r_STATUS_TEMPERATURE    = 0x7d, // Retrieves information about temperature status (R)
    r_STATUS_CML            = 0x7e, // Retrieves information about communications status (R)
    r_STATUS_OTHER          = 0x7f, // Retrieves other status information (R)
    r_STATUS_MFR_SPECIFIC   = 0x80, // Retrieves information about circuit breaker and MOSFET shorted status (R)
    r_READ_EIN              = 0x86, // Retrieves energy meter measurement (R)
    r_READ_VIN              = 0x88, // Retrieves input voltage measurement (R)
    r_READ_IIN              = 0x89, // Retrieves input current measurement (Mirrors at D1h) (R)
    r_READ_VOUT             = 0x8b, // Retrieves output voltage measurement (R)
    r_READ_TEMPERATURE_1    = 0x8d, // Retrieves temperature measurement (R)
    r_READ_PIN              = 0x97, // Retrieves averaged input power measurement (mirror at DFh) (R)
    r_MFR_ID                = 0x99, // Retrieves manufacturer ID in ASCII characters (TI) (R)
    r_MFR_MODEL             = 0x9a, // Retrieves part number in ASCII characters. (LM5066I) (R)
    r_MFR_REVISION          = 0x9b, // Retrieves part revision letter or number in ASCII (for example, AA) (R)
    r_READ_VAUX             = 0xd0, // Retrieves auxiliary voltage measurement (R)
    r_MFR_READ_IIN          = 0xd1, // Retrieves input current measurement (Mirror at 89h) (R)
    r_MFR_READ_PIN          = 0xd2, // Retrieves input power measurement (R)
    r_MFR_IIN_OC_WARN_LIMIT = 0xd3, // Retrieves or stores input current limit warn threshold (Mirror at 5Dh) (R/W)
    r_MFR_PIN_OP_WARN_LIMIT = 0xd4, // Retrieves or stores input power limit warn threshold (R/W)
    r_READ_PIN_PEAK         = 0xd5, // Retrieves measured peak input power measurement (R)
    r_CLEAR_PIN_PEAK        = 0xd6, // Resets the contents of the peak input power register to 0 (send byte)
    r_GATE_MASK             = 0xd7, // Allows the user to disable MOSFET gate shutdown for various fault conditions (R/W)
    r_ALERT_MASK            = 0xd8, // Retrieves or stores user SMBA fault mask (R/W)
    r_DEVICE_SETUP          = 0xd9, // Retrieves or stores information about number of retry attempts (R/W)
    r_BLOCK_READ            = 0xda, // Retrieves most recent diagnostic and telemetry information in a single transaction (R)
    r_SAMPLES_FOR_AVG       = 0xdb, // Exponent value AVGN for number of samples to be averaged (N = 2^AVGN), range = 00h to 0Ch (R/W)
    r_READ_AVG_VIN          = 0xdc, // Retrieves averaged input voltage measurement (R)
    r_READ_AVG_VOUT         = 0xdd, // Retrieves averaged output voltage measurement (R)
    r_READ_AVG_IIN          = 0xde, // Retrieves averaged input current measurement (R)
    r_READ_AVG_PIN          = 0xdf, // Retrieves averaged input power measurement (R)
    r_BLACK_BOX_READ        = 0xe0, // Captures diagnostic and telemetry information, which are latched when the first SMBA event occurs after faults are cleared (R)
    r_DIAGNOSTIC_WORD_READ  = 0xe1, // Manufacturer-specific parallel of the STATUS_WORD to convey all FAULT/WARN data in a single transaction (R)
    r_AVG_BLOCK_READ        = 0xe2  // Retrieves most recent average telemetry and diagnostic information in a single transaction (R)
};

enum status {
    // TODO
};

class LM5066 {

public:

LM5066(i2c_inst_t *i2c, uint8_t SDA_PIN, uint8_t SCL_PIN, uint8_t address);

bool begin(bool pullup, uint clk);

void enable(void);
void disable(void);

void clear_faults(void);


private:

void OPERATION(bool);

void CLEAR_FAULTS(void);

uint CAPABILITY(void);

uint16_t VOUT_UV_WARN_LIMIT(uint16_t limit);

uint16_t OT_FAULT_LIMIT(uint16_t limit);

uint16_t OT_WARN_LIMIT(uint16_t limit);

uint16_t VIN_OV_WARN_LIMIT(uint16_t limit);

uint16_t VIN_UV_WARN_LIMIT(uint16_t limit);

uint IIN_OC_WARN_LIMIT(uint);

uint STATUS_BYTE(void);

uint STATUS_WORD(void);

uint STATUS_VOUT(void);

uint STATUS_INPUT(void);

uint STATUS_TEMPERATURE(void);

uint STATUS_CML(void);

uint STATUS_OTHER(void);

uint STATUS_MFR_SPECIFIC(void);

float READ_EIN(void);

float READ_VIN(void);

float READ_IIN(void);

float READ_VOUT(void);

float READ_TEMPERATURE_1(void);

float READ_PIN(void);

uint MFR_ID(void);

uint MFR_MODEL(void);

uint MFR_REVISION(void);

float READ_VAUX(void);

float MFR_READ_IIN(void);

float MFR_READ_PIN(void);

uint MFR_IIN_OC_WARN_LIMIT(uint);

uint MFR_PIN_OP_WARN_LIMIT(uint);

float READ_PIN_PEAK(void);

void CLEAR_PIN_PEAK(void);

void GATE_MASK(uint);

void ALERT_MASK(uint);

void DEVICE_SETUP(uint);

uint BLOCK_READ(void);

uint SAMPLES_FOR_AVG(uint);

float READ_AVG_VIN(void);

float READ_AVG_VOUT(void);

float READ_AVG_IIN(void);

float READ_AVG_PIN(void);

uint BLACK_BOX_READ(void);

uint READ_DIAGNOSTIC_WORD(void);

uint* AVG_BLOCK_READ(void);

uint8_t _address;
i2c_inst_t *_i2c;
uint8_t _SDA_PIN;
uint8_t _SCL_PIN;

};

#endif // LM5066_h