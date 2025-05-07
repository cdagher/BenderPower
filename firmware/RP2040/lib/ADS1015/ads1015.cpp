
#include "ads1015.h"
#include <cmath>

uint16_t ads1015_config_to_int(struct ads1015_config * config) {
   return 
      ((config->operational_status  & 0x1) << 15) |
      ((config->multiplexer_config  & 0x7) << 12) |
      ((config->gain_amplifier      & 0x7) <<  9) |
      ((config->operating_mode      & 0x1) <<  8) |
      ((config->data_rate           & 0x7) <<  5) |
      ((config->comparator_mode     & 0x1) <<  4) |
      ((config->comparator_polarity & 0x1) <<  3) |
      ((config->latching_comparator & 0x1) <<  2) |
      ((config->comparator_assert   & 0x3) <<  0);
}

void ads1015_config_default(struct ads1015_config * config) {
   config->operational_status  = ADS1015_OS_NOTHING;
   config->multiplexer_config  = ADS1015_MUX_AIN0_AIN1;
   config->gain_amplifier      = ADS1015_PGA_2_048;
   config->operating_mode      = ADS1015_MODE_ONESHOT;
   config->data_rate           = ADS1015_DR_1600SPS;
   config->comparator_mode     = ADS1015_COMP_MODE_TRADITIONAL;
   config->comparator_polarity = ADS1015_COMP_POL_LOW;
   config->latching_comparator = ADS1015_COMP_LAT_OFF;
   config->comparator_assert   = ADS1015_COMP_DISABLE;
}

ads1015::ads1015(i2c_inst_t * i2c_conn, uint8_t ads_addr) {
   this->i2c_conn = i2c_conn;
   this->ads_addr = ads_addr;
   this->have_alert = false;
   this->alert_pin = -1;
}

ads1015::ads1015(i2c_inst_t * i2c_conn, uint8_t ads_addr, uint32_t alert_pin) {
   this->i2c_conn = i2c_conn;
   this->ads_addr = ads_addr;
   this->have_alert = true;
   this->alert_pin = alert_pin;
}

ads1015::~ads1015() {

   set_default_config();

}

uint16_t ads1015::read_conversion() {

   /* indicate read from the given address */
   i2c_write_byte_raw(i2c_conn,((ads_addr<<1) | 1));

   /* choose target as conversion register */
   i2c_write_byte_raw(i2c_conn,ADS1015_REG_CONVERSION);

   uint8_t res[2];

   /* recieve the most significant byte from
    * the conversion register                */
   res[0] = i2c_read_byte_raw(i2c_conn);

   /* recieve the least significant byte from
    * the conversion register                */
   res[1] = i2c_read_byte_raw(i2c_conn);

   return ((((uint16_t)res[0]) << 8) | res[1]);
}

uint16_t ads1015::read_config() {

   /* indicate read from the given address */
   i2c_write_byte_raw(i2c_conn,((ads_addr<<1) | 1));

   /* choose target as config register */
   i2c_write_byte_raw(i2c_conn,ADS1015_REG_CONFIGURATION);

   uint8_t res[2];

   /* recieve the most significant byte from
    * the config register                */
   res[0] = i2c_read_byte_raw(i2c_conn);

   /* recieve the least significant byte from
    * the config register                */
   res[1] = i2c_read_byte_raw(i2c_conn);

   return ((((uint16_t)res[0]) << 8) | res[1]);
}

uint16_t ads1015::read_lothresh() {

   /* indicate read from the given address */
   i2c_write_byte_raw(i2c_conn,((ads_addr<<1) | 1));

   /* choose target as low theshold register */
   i2c_write_byte_raw(i2c_conn,ADS1015_REG_LOW_THRESHOLD);

   uint8_t res[2];

   /* recieve the most significant byte from
    * the low threshold register                */
   res[0] = i2c_read_byte_raw(i2c_conn);

   /* recieve the least significant byte from
    * the low threshold register                */
   res[1] = i2c_read_byte_raw(i2c_conn);

   return ((((uint16_t)res[0]) << 8) | res[1]);
}

uint16_t ads1015::read_hithresh() {

   /* indicate read from the given address */
   i2c_write_byte_raw(i2c_conn,((ads_addr<<1) | 1));

   /* choose target as high threshold register */
   i2c_write_byte_raw(i2c_conn,ADS1015_REG_HIGH_THRESHOLD);

   uint8_t res[2];

   /* recieve the most significant byte from
    * the high threshold register                */
   res[0] = i2c_read_byte_raw(i2c_conn);

   /* recieve the least significant byte from
    * the high threshold register                */
   res[1] = i2c_read_byte_raw(i2c_conn);

   return ((((uint16_t)res[0]) << 8) | res[1]);
}

void ads1015::write_config(uint16_t val) {

   uint8_t msb = val >> 8;
   uint8_t lsb = val & 0xff;

   /* indicate write to the given address */
   i2c_write_byte_raw(i2c_conn,((ads_addr<<1) | 0));

   /* choose target as config register */
   i2c_write_byte_raw(i2c_conn,ADS1015_REG_CONFIGURATION);

   /* write most significant byte to the config register */
   i2c_write_byte_raw(i2c_conn, msb);

   /* write least significant byte to the config register */
   i2c_write_byte_raw(i2c_conn, msb);
}

void ads1015::write_lothresh(uint16_t val) {

   uint8_t msb = val >> 8;
   uint8_t lsb = val & 0xff;

   /* indicate write to the given address */
   i2c_write_byte_raw(i2c_conn,((ads_addr<<1) | 0));

   /* choose target as low threshold register */
   i2c_write_byte_raw(i2c_conn,ADS1015_REG_LOW_THRESHOLD);

   /* write most significant byte to the low threshold register */
   i2c_write_byte_raw(i2c_conn, msb);

   /* write least significant byte to the low threshold register */
   i2c_write_byte_raw(i2c_conn, msb);
}

void ads1015::write_hithresh(uint16_t val) {

   uint8_t msb = val >> 8;
   uint8_t lsb = val & 0xff;

   /* indicate write to the given address */
   i2c_write_byte_raw(i2c_conn,((ads_addr<<1) | 0));

   /* choose target as high threshold register */
   i2c_write_byte_raw(i2c_conn,ADS1015_REG_HIGH_THRESHOLD);

   /* write most significant byte to the high threshold register */
   i2c_write_byte_raw(i2c_conn, msb);

   /* write least significant byte to the high threshold register */
   i2c_write_byte_raw(i2c_conn, msb);
}

void ads1015::set_default_config() {

   ads1015_config_default(&config);

   write_config(ads1015_config_to_int(&config));

}

void ads1015::set_continous_conversion() {

   stop_conversion();

   config.operating_mode = ADS1015_MODE_CONTINUOUS;

   write_config(ads1015_config_to_int(&config));

}

void ads1015::set_alert_on() {

   if ( !have_alert )
      return;

   write_hithresh(0b1000000000000000);
   write_lothresh(0b0000000000000000);

   if ((read_config() & 0x3) == ADS1015_COMP_DISABLE) {
      config.comparator_assert = ADS1015_COMP_QUE_ONE;
      write_config(ads1015_config_to_int(&config));
   }

}

void ads1015::set_alert_off() {

   if ( !have_alert )
      return;

   write_hithresh(0b0000000000000000);
   write_lothresh(0b1000000000000000);

   disable_comparator();
}

void ads1015::start_oneshot_conversion() {

   config.operating_mode = ADS1015_MODE_ONESHOT;
   config.operational_status = ADS1015_OS_SINGLE;

   write_config(ads1015_config_to_int(&config));

   /* wait for the oneshot conversion to start */
   sleep_us(50);

}

void ads1015::stop_conversion() {

   config.operating_mode = ADS1015_MODE_ONESHOT;

   write_config(ads1015_config_to_int(&config));

}

int ads1015::alert() {

   if (have_alert) {

      return gpio_get(alert_pin);

   }

   return 0;
}

int ads1015::is_converting() {

   uint16_t status = read_config();

   return !( status >> 31 );

}

double ads1015::set_gain_best_effort(double gain) {

   int target;

   /* note that the values we are checking are slightly higher than the 
    * values we are setting to. This is done because I am scared of the 
    * floating point conversions rounding off and the gain being set to
    * too high of a value                                              */

   gain = (gain < 0 ? gain * -1 : gain);

   if      (gain <= 0.256005)
      target = ADS1015_PGA_0_256; 
   else if (gain <= 0.512005)
      target = ADS1015_PGA_0_512;
   else if (gain <= 1.024005)
      target = ADS1015_PGA_1_024;
   else if (gain <= 2.048005)
      target = ADS1015_PGA_2_048;
   else if (gain <= 4.096005)
      target = ADS1015_PGA_4_096;
   else 
      target = ADS1015_PGA_6_114;

   return set_gain(target);
}

double ads1015::set_gain(uint8_t gain) {

   if (gain > 0b111)
      gain = 0b111;

   config.gain_amplifier = gain;

   write_config(ads1015_config_to_int(&config));

   switch (gain) {
      case 0b000:
         return 6.114;
         break;
      case 0b001:
         return 4.096;
         break;
      case 0b010:
         return 2.048;
         break;
      case 0b011:
         return 1.024;
         break;
      case 0b100:
         return 0.512;
         break;
      case 0b101:
      case 0b110:
      case 0b111:
      default:
         return 0.256;
   }
}

void ads1015::enable_comparator(uint8_t trigger_num, uint16_t high_thresh, uint16_t low_thresh, bool latching) {

   if (high_thresh <= low_thresh)
      return;

   if (trigger_num > 0b10)
      trigger_num = 0b10;

   if (high_thresh > 0xfff)
      high_thresh = 0xfff;

   if (low_thresh > 0xfff)
      low_thresh = 0xfff;

   write_hithresh(high_thresh << 4);
   write_lothresh(low_thresh  << 4);

   config.comparator_assert = trigger_num;

   if (latching)
      config.latching_comparator = ADS1015_COMP_LAT_ON;
   else 
      config.latching_comparator = ADS1015_COMP_LAT_OFF;

   write_config(ads1015_config_to_int(&config));
}

void ads1015::disable_comparator() {

   config.comparator_assert = ADS1015_COMP_DISABLE;

   write_config(ads1015_config_to_int(&config));

}

void ads1015::set_config() {

   write_config(ads1015_config_to_int(&config));

}

void ads1015::set_multiplexer(uint8_t mux) {

   if (mux > 0b111)
      mux = 0b111;

   config.multiplexer_config = mux;

   write_config(ads1015_config_to_int(&config));
}

void ads1015::set_comparator_polarity(bool status) {

   if (status) 
      config.comparator_polarity = ADS1015_COMP_POL_HIGH;
   else
      config.comparator_polarity = ADS1015_COMP_POL_LOW;

   write_config(ads1015_config_to_int(&config));
}

void ads1015::set_comparator_window(bool status) {

   if (status)
      config.comparator_mode = ADS1015_COMP_MODE_WINDOW;
   else
      config.comparator_mode = ADS1015_COMP_MODE_TRADITIONAL;

   write_config(ads1015_config_to_int(&config));
}

uint16_t ads1015::set_data_rate(uint8_t dr) {

   if (dr > 0b111)
      dr = 0b111;

   config.data_rate = dr;

   write_config(ads1015_config_to_int(&config));

   switch (dr) {
      case 0b000:
         return 128;
         break;
      case 0b001:
         return 250;
         break;
      case 0b010:
         return 490;
         break;
      case 0b011:
         return 920;
         break;
      case 0b100:
         return 1600;
         break;
      case 0b101:
         return 2400;
         break;
      case 0b110:
      case 0b111:
      default:
         return 3300;
         break;
   }
}

uint16_t ads1015::set_data_rate_best_effort(uint16_t dr) {

   if      (dr <= 128)
      dr = ADS1015_DR_128SPS;
   else if (dr <= 250)
      dr = ADS1015_DR_250SPS;
   else if (dr <= 490)
      dr = ADS1015_DR_490SPS;
   else if (dr <= 920)
      dr = ADS1015_DR_920SPS;
   else if (dr <= 1600)
      dr = ADS1015_DR_1600SPS;
   else if (dr <= 2400)
      dr = ADS1015_DR_2400SPS;
   else 
      dr = ADS1015_DR_3300SPS;

   return set_data_rate((uint8_t)dr);
}

