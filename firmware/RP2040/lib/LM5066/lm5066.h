
#ifndef __RP2040_FIRMWARE_LIB_LM5066__

#define __RP2040_FIRMWARE_LIB_LM5066__

#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include <stdint.h>
#include <math.h>

/* Retrieves or stores the operation status */
#define LM5066_CMD_OPERATION 0x01
/* switch on */
#define LM5066_OPERATION_ON  0x80
/* switch off */
#define LM5066_OPERATION_OFF 0x00

/* Clears the status registers and re-arms the black box registers for updating  */
#define LM5066_CMD_CLEAR_FAULTS 0x03

/* Retrieves the device capability */
#define LM5066_CMD_CAPABILITY 0x19
/* the response returned by the above command */
#define LM5066_CAPABILITY_RESPONSE 0xb0

/* Retrieves or stores output undervoltage warn limit threshold */
#define LM5066_CMD_VOUT_UV_WARN_LIMIT 0x43
/* disabled value for above command */
#define LM5066_VOUT_UV_WARN_LIMIT_OFF 0x0000

/* Retrieves or stores over temperature fault limit threshold */
#define LM5066_CMD_OT_FAULT_LIMIT 0x4f
/* disabled value for above command */
#define LM5066_OT_FAULT_LIMIT_OFF 0x0fff

/* Retrieves or stores over temperature warn limit threshold */
#define LM5066_CMD_OT_WARN_LIMIT 0x51
/* disabled value for above command */
#define LM5066_OT_WARN_LIMIT_OFF 0x0fff

/* Retrieves or stores input overvoltage warn limit threshold */
#define LM5066_CMD_VIN_OV_WARN_LIMIT 0x57
/* disabled value for above command */
#define LM5066_VIN_OV_WARN_LIMIT_OFF 0x0fff

/* Retrieves or stores input undervoltage warn limit threshold */
#define LM5066_CMD_VIN_UV_WARN_LIMIT 0x58
/* disabled value for above command */
#define LM5066_VIN_UV_WARN_LIMIT_OFF 0x0000

/* Retrieves or stores input current warn limit threshold (mirror at D3h) */
#define LM5066_CMD_IIN_OC_WARN_LIMIT 0x5d
/* there is no data on what this means???? */

/* Retrieves information about the parts operating status */
#define LM5066_CMD_STATUS_BYTE 0x78
/* not supported, always zero */
#define LM5066_STATUS_BYTE_BUSY         0x80
/* This bit is asserted if the MOSFET is not switched on for any reason */
#define LM5066_STATUS_BYTE_OFF          0x40
/* not supported, always zero */
#define LM5066_STATUS_BYTE_VOUT_OV      0x20
/* not supported, always zero */
#define LM5066_STATUS_BYTE_IOUT_OC      0x10
/* a VIN undervoltage fault has occurred */
#define LM5066_STATUS_BYTE_VIN_UV_FAULT 0x08
/* a temperature fault or warning has occurred */
#define LM5066_STATUS_BYTE_TEMPERATURE  0x04
/* a communication fault has occurred */
#define LM5066_STATUS_BYTE_CML          0x02
/* a fault or warning not listed in bits 7-1 has occured */
#define LM5066_STATUS_BYTE_NONE         0x01

/* Retrieves information about the parts operating status */
#define LM5066_CMD_STATUS_WORD 0x79
/* an output voltage fault or warning has occured */
#define LM5066_STATUS_WORD_VOUT       0x8000
/* not supported, always zero */
#define LM5066_STATUS_WORD_IOUT_POUT  0x4000
/* an input voltage or current fault has occurred */
#define LM5066_STATUS_WORD_INPUT      0x2000
/* FET is shorted */
#define LM5066_STATUS_WORD_FET_FAIL   0x1000
/* the power good signal has been negated */
#define LM5066_STATUS_WORD_POWER_GOOD 0x0800
/* not supported, always zero */
#define LM5066_STATUS_WORD_FANS       0x0400
/* circuit breaker fault triggered */
#define LM5066_STATUS_WORD_CB_FAULT   0x0200
/* not supported, always zero */
#define LM5066_STATUS_WORD_UNKNOWN    0x0100

/* Retrieves information about output voltage status */
#define LM5066_CMD_STATUS_VOUT 0x7a
/* a VOUT undervoltage warning has occured */
#define LM5066_STATUS_VOUT_UV_WARN 0x20

/* Retrieves information about input status */
#define LM5066_CMD_STATUS_INPUT 0x7c
/* a VIN overvoltage fault has occurred */
#define LM5066_STATUS_INPUT_VIN_OV_FAULT 0x80
/* a VIN overvoltage warning has occurred */
#define LM5066_STATUS_INPUT_VIN_OV_WARN  0x40
/* a VIN undervoltage warning has occurred */
#define LM5066_STATUS_INPUT_VIN_UV_WARN  0x20
/* a VIN undervoltage fault has occurred */
#define LM5066_STATUS_INPUT_VIN_UV_FAULT 0x10
/* an IIN overcurrent fault has occurred */
#define LM5066_STATUS_INPUT_IIN_OC_FAULT 0x04
/* an IIN overcurrent warning has occurred */
#define LM5066_STATUS_INPUT_IIN_OC_WARN  0x02
/* an PIN overpower warning has occurred */
#define LM5066_STATUS_INPUT_PIN_OP_WARN  0x01

/* Retrieves information about temperature status */
#define LM5066_CMD_STATUS_TEMPERATURE 0x7d
/* an overtemperature fault has occured */
#define LM5066_STATUS_TEMPERATURE_OVERTEMP_FAULT 0x80
/* an overtemperature warning has occured */
#define LM5066_STATUS_TEMPERATURE_OVERTEMP_WARN  0x40

/* Retrieves information about communications status */
#define LM5066_CMD_STATUS_CML 0x7e
/* invalid or unsupported command recieved */
#define LM5066_STATUS_CML_INV_CMD   0x80
/* invalid or unsupported data recieved */
#define LM5066_STATUS_CML_INV_DATA  0x40
/* packet error check failed */
#define LM5066_STATUS_CML_PACKET    0x20
/* miscellaneous communications fault has occurred */
#define LM5066_STATUS_CML_COM_FAULT 0x02

/* Retrieves other status information */
#define LM5066_CMD_STATUS_OTHER 0x7f
/* CB fault */
#define LM5066_CMD_STATUS_OTHER_CB 0x20

/* Retrieves information about circuit breaker and MOSFET shorted status */
#define LM5066_CMD_STATUS_MFR_SPECIFIC 0x80
/* circuit breaker fault */
#define LM5066_STATUS_MFR_CIRCUIT_BREAKER 0x80
/* external mosfet shorted fault */
#define LM5066_STATUS_MFR_MOSFET_SHORTED  0x40
/* defaults loaded */
#define LM5066_STATUS_MFR_DEFAULTS        0x10

/* Retrieves energy meter measurement */
#define LM5066_CMD_READ_EIN 0x86

/* Retrieves input voltage measurement */
#define LM5066_CMD_READ_VIN 0x88

/* Retrieves input current measurement (Mirrors at D1h) */
#define LM5066_CMD_READ_IIN 0x89

/* Retrieves output voltage measurement */
#define LM5066_CMD_READ_VOUT 0x8b

/* Retrieves temperature measurement */
#define LM5066_CMD_READ_TEMPERATURE_1 0x8d

/* Retrieves averaged input power measurement (mirror at DFh). */
#define LM5066_CMD_READ_PIN 0x97

/* Retrieves manufacturer ID in ASCII characters (TI) */
#define LM5066_CMD_MFR_ID 0x99

/* Retrieves part number in ASCII characters. */
#define LM5066_CMD_MFR_MODEL 0x9a

/* Retrieves part revision letter or number in ASCII (for example, AA) */
#define LM5066_CMD_MFR_REVISION 0x9b

/* Retrieves auxiliary voltage measurement */
#define LM5066_CMD_READ_VAUX 0xd0

/* Retrieves input current measurement (Mirror at 89h) */
#define LM5066_CMD_MFR_READ_IIN 0xd1

/* Retrieves input power measurement */
#define LM5066_CMD_MFR_READ_PIN 0xd2

/* Retrieves or stores input current limit warn threshold (Mirror at 5Dh) */
#define LM5066_CMD_MFR_IIN_OC_WARN_LIMIT 0xd3

/* Retrieves or stores input power limit warn threshold */
#define LM5066_CMD_MFR_PIN_OP_WARN_LIMIT 0xd4

/* Retrieves measured peak input power measurement */
#define LM5066_CMD_READ_PIN_PEAK 0xd5

/* Resets the contents of the peak input power register to 0 */
#define LM5066_CMD_CLEAR_PIN_PEAK  0xd6

/* Allows the user to disable MOSFET gate shutdown for various fault
   conditions */
#define LM5066_CMD_GATE_MASK 0xd7

/* Retrieves or stores user SMBA fault mask */
#define LM5066_CMD_ALERT_MASK 0xd8

/* Retrieves or stores information about number of retry attempts */
#define LM5066_CMD_DEVICE_SETUP 0xd9

/* Retrieves most recent diagnostic and telemetry information in a
   single transaction */
#define LM5066_CMD_BLOCK_READ 0xda

/* Exponent value AVGN for number of samples to be averaged (N =
   2AVGN), range = 00h to 0Ch */
#define LM5066_CMD_SAMPLES_FOR_AVG 0xdb

/* Retrieves averaged input voltage measurement */
#define LM5066_CMD_READ_AVG_VIN 0xdc

/* Retrieves averaged output voltage measurement */
#define LM5066_CMD_READ_AVG_VOUT 0xdd

/* Retrieves averaged input current measurement */
#define LM5066_CMD_READ_AVG_IIN 0xde

/* Retrieves averaged input power measurement */
#define LM5066_CMD_READ_AVG_PIN 0xdf

/* Captures diagnostic and telemetry information, which are latched
   when the first SMBA event occurs after faults are cleared */
#define LM5066_CMD_BLACK_BOX_READ 0xe0

/* Manufacturer-specific parallel of the STATUS_WORD to convey all
   FAULT/WARN data in a single transaction */
#define LM5066_CMD_DIAGNOSTIC_WORD_READ 0xe1

/* Retrieves most recent average telemetry and diagnostic information
in a single transaction */
#define LM5066_CMD_AVG_BLOCK_READ 0xe2

class lm5066 {

private:

   i2c_inst_t * i2c_conn;
   uint8_t addr;
   uint32_t smba_pin;

   uint16_t diagnostic_word = 0;
   uint16_t avg_in = 0;
   uint16_t avg_vout = 0;
   uint16_t avg_vin = 0;
   uint16_t avg_pin = 0;
   uint16_t temperature = 0;

   /* send the given command as read(false) or write(true) */
   void send_command(uint8_t cmd, bool rw);

   /* write the given bytes to the device */
   void write_data(uint8_t * bytes, int num);

   /* read the given bytes from the device */
   void read_data(uint8_t * bytes, int num);

   /* write the given command and then write the given bytes */
   void cmd_read(uint8_t cmd, uint8_t * bytes, int num);

   /* write the given command and then read the given bytes */
   void cmd_write(uint8_t cmd, uint8_t * bytes, int num);

   /* convert to a uint16_t from double using the formula on pg. 39
    * of the lm5066 TI datasheet                                   */
   uint16_t to_telemetry_int(double val, double m, double b, int R);

   /* perform the inverse of the above operation */
   double from_telemetry_int(uint16_t val, double m, double b, int R);

public:

   /* initialize the lm5066 */
   lm5066(i2c_inst_t * i2c_conn, uint8_t addr, uint32_t smba_pin);

   /* reset all values to default on exit */
   ~lm5066();

   /* switch the mosfet off and then back on */
   void reenable();

   /* set the mosfet as off or on */
   void set_mosfet(bool status);

   /* get the mosfet status. true is on, false is off */
   bool get_mosfet();

   /* clear faults on the device. You may want to set the mosfet 
    * on after this as it might be off                          */
   void clear_faults();

   /* disable the undervoltage warning detection feature */
   void disable_vout_undervoltage_warn();

   /* set the threshold for vout undervoltage warning */
   void set_vout_undervoltage_warn(double volts);

   /* get the threshold for vout undervoltage warning */
   double get_vout_undervoltage_warn();

   /* disable the overtemperature fault feature */
   void disable_overtemperature_fault();

   /* set the threshold for overtemperature fault */
   void set_overtemperature_fault_val(double degrees_c);

   /* get the threshold for overtemperature fault */
   double get_overtemperature_fault_val();

   /* disable the overtemperature warn feature */
   void disable_overtemperature_warn();

   /* set the threshold for overtemperature warn */
   void set_overtemperature_warn_val(double degrees_c);

   /* get the threshold for overtemperature warn */
   double get_overtemperature_warn_val();

   /* disable the overtemperature warn feature */
   void disable_overvoltage_warn();

   /* set the threshold for overvoltage warning */
   void set_overvoltage_warn_val(double volts);

   /* get the threshold for overvoltage warning */
   double get_overvoltage_warn_val();

   /* disable the undervoltage warn feature */
   void disable_undervoltage_warn();

   /* set the threshold for undervoltage warn */
   void set_undervoltage_warn_val(double volts);

   /* get the threshold for undervoltage warn */
   double get_undervoltage_warn_val();

private:

   struct lm5066_ein_values {
      uint32_t accumulator;
      uint32_t sample_count;
   };

   struct lm5066_ein_values prev_ein = { 0, 0 };
   struct lm5066_ein_values curr_ein = { 0, 0 };

public:

   /* return the average input power consumption 
    * Note that because of how this value works you
    * are going to want to call it, sleep for a bit,
    * and then call it again if you have not called it
    * in a while to ensure that no more than a single 
    * overflow has occured. So if you haven't called it
    * for more than 3 or 4 seconds call it once, sleep
    * for a few millseconds and then call it again and
    * take the value that is returned from the second call */
   double get_ein();

   /* do everything discussed above */
   double get_ein_safe();

   /* return the current reading of input voltage */
   double get_vin();

   /* return the current reading of input amperage */
   double get_iin();

   /* return the current reading of the output voltage */
   double get_vout();

   /* return the current reading of the device temperature */
   double get_temperature();
   
   /* return current reading of input power */
   double get_pin();

   /* return a 14-bit string describing this device
    * ... manufacturer, name, revision ...         
    * If size is less than 14, nothing happens. Last
    * byte will be a null one                      */
   void get_part_string(char * buf, int size);

   /* get the measured auxillary voltage */
   double get_vaux();

   /* disable the overcurrent warn feature */
   void disable_overcurrent_warn();

   /* set the threshold for overvcurrent warn */
   void set_overcurrent_warn_val(double amps);

   /* get the threshold for overcurrent warn */
   double get_overcurrent_warn_val();

   /* disable the overpower warn feature */
   void disable_overpower_warn();

   /* set the threshold for overpower warn */
   void set_overpower_warn_val(double watts);

   /* get the threshold for overpower warn */
   double get_overpower_warn_val();



};


#endif
