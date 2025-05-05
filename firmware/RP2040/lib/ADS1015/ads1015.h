
#ifndef __RP2040_FIRMWARE_LIB_ADS1015__

#define __RP2040_FIRMWARE_LIB_ADS1015__

#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include <stdint.h>

/* =============================================
 * operational status options for the ads1015 
 * =============================================*/
/* do nothing */
#define ADS1015_OS_NOTHING 0b0
/* start a single conversion when powered down */
#define ADS1015_OS_SINGLE  0b1

/* =============================================
 * multiplexer configuration options for the ads1015 
 * =============================================*/
/* I don't really know what this does but this is 
 * what this sets... xD                          */
#define ADS1015_MUX_AIN0_AIN1 0b000
#define ADS1015_MUX_AIN0_AIN3 0b001
#define ADS1015_MUX_AIN1_AIN3 0b010
#define ADS1015_MUX_AIN2_AIN3 0b011
#define ADS1015_MUX_AIN0_GND  0b100
#define ADS1015_MUX_AIN1_GND  0b101
#define ADS1015_MUX_AIN2_GND  0b110
#define ADS1015_MUX_AIN3_GND  0b111

/* =============================================
 * gain amplifier options for the ads1015 
 * =============================================*/
/* set full scale range to +- 6.144 V */
#define ADS1015_PGA_6_114 0b000
/* set full scale range to +- 4.096 V */
#define ADS1015_PGA_4_096 0b001
/* set full scale range to +- 2.048 V */
#define ADS1015_PGA_2_048 0b010
/* set full scale range to +- 1.024 V */
#define ADS1015_PGA_1_024 0b011
/* set full scale range to +- 0.512 V */
#define ADS1015_PGA_0_512 0b100
/* set full scale range to +- 0.256 V */
#define ADS1015_PGA_0_256 0b111

/* =============================================
 * operating mode options for the ads1015 
 * =============================================*/
/* continuous conversion mode */
#define ADS1015_MODE_CONTINUOUS 0b0
/* one-shot/power-down mode. Set OS_SINGLE in
 * operational_status to start the one-shot conversion */
#define ADS1015_MODE_ONESHOT    0b1

/* =============================================
 * data rate options for the ads1015 
 * =============================================*/
/* set to 128 samples per second */
#define ADS1015_DR_128SPS  0b000
/* set to 250 samples per second */
#define ADS1015_DR_250SPS  0b001
/* set to 490 samples per second */
#define ADS1015_DR_490SPS  0b010
/* set to 920 samples per second */
#define ADS1015_DR_920SPS  0b011
/* set to 1600 samples per second */
#define ADS1015_DR_1600SPS 0b100
/* set to 2400 samples per second */
#define ADS1015_DR_2400SPS 0b101
/* set to 3300 samples per second */
#define ADS1015_DR_3300SPS 0b111

/* =============================================
 * comparator mode options for the ads1015 
 * =============================================*/
/* set the comparator operating mode to traditional */
#define ADS1015_COMP_MODE_TRADITIONAL 0b0
/* set the comparator operating mode to window */
#define ADS1015_COMP_MODE_WINDOW 0b1

/* =============================================
 * comparator polarity options for the ads1015 
 * =============================================*/
/* set the comparator polarity to active low */
#define ADS1015_COMP_POL_LOW  0b0
/* set the comparator polarity to active high */
#define ADS1015_COMP_POL_HIGH 0b1

/* =============================================
 * latching comparator options for the ads1015 
 * =============================================*/
/* set comparator as nonlatching */
#define ADS1015_COMP_LAT_OFF 0b0
/* set comparator as latching */
#define ADS1015_COMP_LAT_ON  0b1

/* =============================================
 * comparator queue options for the ads1015 
 * =============================================*/
/* set the alert/rdy pin after one conversion */
#define ADS1015_COMP_QUE_ONE  0b00
/* set the alert/rdy pin after two conversions */
#define ADS1015_COMP_QUE_TWO  0b01
/* set the alert/rdy pin after four conversions */
#define ADS1015_COMP_QUE_FOUR 0b10
/* disable the comparator */
#define ADS1015_COMP_DISABLE  0b11

/* =============================================
 *  registers for the ads1015 
 * =============================================*/
/* the conversion result register */
#define ADS1015_REG_CONVERSION     0b00
/* the config register */
#define ADS1015_REG_CONFIGURATION  0b01
/* the lo threshold register */
#define ADS1015_REG_LOW_THRESHOLD   0b10
/* the high threshold register */
#define ADS1015_REG_HIGH_THRESHOLD 0b11


struct ads1015_config {
   int operational_status  = ADS1015_OS_NOTHING;
   int multiplexer_config  = ADS1015_MUX_AIN0_AIN1;
   int gain_amplifier      = ADS1015_PGA_2_048;
   int operating_mode      = ADS1015_MODE_ONESHOT;
   int data_rate           = ADS1015_DR_1600SPS;
   int comparator_mode     = ADS1015_COMP_MODE_TRADITIONAL;
   int comparator_polarity = ADS1015_COMP_POL_LOW;
   int latching_comparator = ADS1015_COMP_LAT_OFF;
   int comparator_assert   = ADS1015_COMP_DISABLE;
};

/* generate an integer mapping to the ads1015 register from the 
 * given ads1015_config struct                                 */
uint16_t ads1015_config_to_int(struct ads1015_config * config);

/* reset the given ads1015_config struct to defaults */
void ads1015_config_default(struct ads1015_config * config);

class ads1015 {
private:

   /* the i2c connection we are communicating over */
   i2c_inst_t * i2c_conn;
   /* the address of the ads1015 we are communicating with */
   uint8_t ads_addr;
   /* whether the alert pin is provided, as well as the alert pin */
   bool have_alert;
   uint32_t alert_pin;

public:

   struct ads1015_config config;

   /* instantiate a new instance of the ads1015 controller with
    * the given i2c instance and address                       */
   ads1015(i2c_inst_t * i2c_conn, uint8_t ads_addr);

   /* instantiate a new instance of the ads1015 controller with
    * the given i2c instance and address. Also provide the
    * alert pin for status updates                           */
   ads1015(i2c_inst_t * i2c_conn, uint8_t ads_addr, uint32_t alert_pin);

   /* mainly just set the adc to default when the class
    * instance goes out of scope.                       */
   ~ads1015();

   /* read contents of the conversion register */
   uint16_t read_conversion();

   /* read contents of the config register */
   uint16_t read_config();
   /* write contents of the config register */
   void write_config(uint16_t val);

   /* read contents of the lo thresh register */
   uint16_t read_lothresh();
   /* write contents of the lo thresh register */
   void write_lothresh(uint16_t val);

   /* read contents of the hi thresh register */
   uint16_t read_hithresh();
   /* write contents of the hi thresh register */
   void write_hithresh(uint16_t val);

   /* return the adc to it's default settings */
   void set_default_config();

   /* set the adc to start continuously converting */
   void set_continous_conversion();

   /* set the appropriate values to enable the alert/rdy
    * pin to work. This will take the use of the comparator */
   void set_alert_on();
   /* set the appropriate values to disable the alert/rdy
    * pin from working. This will revert any values used
    * to their defaults.                                 */
   void set_alert_off();

   /* set the adc to one-shot conversion mode and start a 
    * one-shot conversion.                               */
   void start_oneshot_conversion();

   /* stop any conversions */
   void stop_conversion();

   /* return the status of the alert pin */
   int alert();

   /* set the gain to the closest value equal or higher 
    * then the provided gain. Return the actual gain set. */
   double set_gain_best_effort(double gain);

   /* set the gain on the adc using the gain macros.
    * Return the gain set.                          */
   double set_gain(int gain);

   /* turn on the comparator with the given high and low
    * thresholds. This will override the previous use
    * of the comparator and implicity start the alert pin 
    * trigger_num is one of the COMP_QUE macros and determines
    * the number of times the value can go out of range before
    * triggering the alert pin. latching determines whether to
    * set the comparator as latching. Default is false - no */
   void enable_comparator(int trigger_num, int high_thresh, int low_thresh, bool latching = false);

   /* turn off the comparator - this will disable the alert pin 
    * if it was being used                                      */
   void disable_comparator();

   /* write the current config struct to the config register */
   void set_config();

};

#endif
