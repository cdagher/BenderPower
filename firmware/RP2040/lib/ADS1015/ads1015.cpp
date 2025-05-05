
#include "ads1015.h"

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

uint16_t ads1015::read_conversion() {

   /* indicate read from the given address */
   i2c_write_byte_raw(i2c_conn,((ads_addr<<1) | 0));

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
   i2c_write_byte_raw(i2c_conn,((ads_addr<<1) | 0));

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
   i2c_write_byte_raw(i2c_conn,((ads_addr<<1) | 0));

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
   i2c_write_byte_raw(i2c_conn,((ads_addr<<1) | 0));

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
   i2c_write_byte_raw(i2c_conn,((ads_addr<<1) | 1));

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
   i2c_write_byte_raw(i2c_conn,((ads_addr<<1) | 1));

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
   i2c_write_byte_raw(i2c_conn,((ads_addr<<1) | 1));

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

