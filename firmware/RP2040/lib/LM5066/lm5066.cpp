#include "lm5066.h"
#include <limits.h>

double exp10(int val) {

   switch( val ) {

      case -10:
         return 1e-10;

      case -9:
         return 1e-9;

      case -8:
         return 1e-8;

      case -7:
         return 1e-7;

      case -6:
         return 1e-6; 

      case -5:
         return 1e-5; 

      case -4:
         return 1e-4; 

      case -3:
         return 1e-3; 

      case -2:
         return 1e-2; 

      case -1:
         return 1e-1; 

      case 0:
         return 1; 

      case 1:
         return 1e1; 

      case 2:
         return 1e2; 

      case 3:
         return 1e3; 

      case 4:
         return 1e4; 

      case 5:
         return 1e5; 

      case 6:
         return 1e6; 

      case 7:
         return 1e7; 

      case 8:
         return 1e8; 

      case 9:
         return 1e9; 

      case 10:
         return 1e10; 

   }
   return 0;
}

lm5066::lm5066(i2c_inst_t * i2c_conn, uint8_t addr, uint32_t smba_pin, float sense_resistor) {
   this->sense_resistor = sense_resistor;
   this->i2c_conn = i2c_conn;
   this->addr = addr;
   this->smba_pin = smba_pin;
}

void lm5066::send_command(uint8_t cmd) {
   // Use i2c_write_blocking to send the command byte
   i2c_write_blocking(i2c_conn, addr, &cmd, 1, false);
}

void lm5066::write_data(uint8_t * bytes, int num) {
   // Use i2c_write_blocking to send the data bytes
   i2c_write_blocking(i2c_conn, addr, bytes, num, false);
}

void lm5066::read_data(uint8_t * bytes, int num) {
   // Use i2c_read_blocking to read the data bytes
   i2c_read_blocking(i2c_conn, addr, bytes, num, false);
}

void lm5066::cmd_read(uint8_t cmd, uint8_t * bytes, int num) {
    // Write the command byte to the device (write phase)
    i2c_write_blocking(i2c_conn, addr, &cmd, 1, true); // true = no stop (repeated start)
    // Now read the data (read phase)
    i2c_read_blocking(i2c_conn, addr, bytes, num, false); // false = send stop after read
}

void lm5066::cmd_write(uint8_t cmd, uint8_t * bytes, int num) {

   send_command(cmd);

   write_data(bytes,num);

}

void lm5066::reenable() {

   set_mosfet(false);

   sleep_ms(10);

   set_mosfet(true);

}

void lm5066::set_mosfet(bool status) {

   uint8_t cmd = (status ? LM5066_OPERATION_ON : LM5066_OPERATION_OFF); 

   cmd_write(LM5066_CMD_OPERATION,&cmd,1);

}

bool lm5066::get_mosfet() {

   uint8_t res;

   cmd_read(LM5066_CMD_OPERATION,&res,1);

   return (res ? true : false);
}

void lm5066::clear_faults() {

   uint8_t send_byte = 0;

   cmd_write(LM5066_CMD_CLEAR_FAULTS,&send_byte,1);

}

void lm5066::disable_vout_undervoltage_warn() {

   uint8_t vals[2] = { 0x00, 0x00 };

   cmd_write(LM5066_CMD_VOUT_UV_WARN_LIMIT,vals,2);

}

uint16_t lm5066::to_telemetry_int(double val, double m, double b, int R) {

   uint32_t res = (((val * m) + b) * exp10((double)(R)));

   if (res > USHRT_MAX)
      res = USHRT_MAX;

   return (uint16_t)res;
}

double lm5066::from_telemetry_int(uint16_t val, double m, double b, int R) {

   double res = (((((double)val) * exp10((double)(-R))) - b) / m);

   return res;
}

void lm5066::set_vout_undervoltage_warn(double volts) {

   uint16_t to_send = to_telemetry_int(volts, 4617, -140.0, -2);
   uint8_t data[2] = { (uint8_t)(to_send & 0xff), (uint8_t)((to_send >> 8)&0xf) };

   cmd_write(LM5066_CMD_VOUT_UV_WARN_LIMIT, data, 2);
}

double lm5066::get_vout_undervoltage_warn() {

   uint8_t recved[2];

   cmd_read(LM5066_CMD_VOUT_UV_WARN_LIMIT, recved, 2);

   uint16_t val = recved[0] | ((uint16_t)recved[1] << 8);

   return from_telemetry_int(val, 4617, -140.0, -2);
}

void lm5066::set_overtemperature_fault_val(double degrees_c) {

   uint16_t to_send = to_telemetry_int(degrees_c, 16000, 0, -3);
   uint8_t data[2] = { (uint8_t)(to_send & 0xff), (uint8_t)((to_send >> 8)&0xf) };

   cmd_write(LM5066_CMD_OT_FAULT_LIMIT, data, 2);
}

double lm5066::get_overtemperature_fault_val() {

   uint8_t recved[2];

   cmd_read(LM5066_CMD_OT_FAULT_LIMIT, recved, 2);

   uint16_t val = recved[0] | ((uint16_t)recved[1] << 8);

   return from_telemetry_int(val, 16000, 0, -3);
}

void lm5066::disable_overtemperature_fault() {

   uint8_t vals[2] = { 0xff, 0x0f };

   cmd_write(LM5066_CMD_OT_FAULT_LIMIT,vals,2);

}

void lm5066::set_overtemperature_warn_val(double degrees_c) {

   uint16_t to_send = to_telemetry_int(degrees_c, 16000, 0, -3);
   uint8_t data[2] = { (uint8_t)(to_send & 0xff), (uint8_t)((to_send >> 8)&0xf) };

   cmd_write(LM5066_CMD_OT_WARN_LIMIT, data, 2);
}

double lm5066::get_overtemperature_warn_val() {

   uint8_t recved[2];

   cmd_read(LM5066_CMD_OT_WARN_LIMIT, recved, 2);

   uint16_t val = recved[0] | ((uint16_t)recved[1] << 8);

   return from_telemetry_int(val, 16000, 0, -3);
}

void lm5066::disable_overtemperature_warn() {

   uint8_t vals[2] = { 0xff, 0x0f };

   cmd_write(LM5066_CMD_OT_WARN_LIMIT,vals,2);

}

void lm5066::set_overvoltage_warn_val(double volts) {

   uint16_t to_send = to_telemetry_int(volts, 4617, -140, -2);
   uint8_t data[2] = { (uint8_t)(to_send & 0xff), (uint8_t)((to_send >> 8)&0xf) };

   cmd_write(LM5066_CMD_VIN_OV_WARN_LIMIT, data, 2);
}

double lm5066::get_overvoltage_warn_val() {

   uint8_t recved[2];

   cmd_read(LM5066_CMD_VIN_OV_WARN_LIMIT, recved, 2);

   uint16_t val = recved[0] | ((uint16_t)recved[1] << 8);

   return from_telemetry_int(val, 4617, -140, -2);
}

void lm5066::disable_overvoltage_warn() {

   uint8_t vals[2] = { 0xff, 0x0f };

   cmd_write(LM5066_CMD_VIN_OV_WARN_LIMIT,vals,2);

}

void lm5066::set_undervoltage_warn_val(double volts) {

   uint16_t to_send = to_telemetry_int(volts, 4617, -140, -2);
   uint8_t data[2] = { (uint8_t)(to_send & 0xff), (uint8_t)((to_send >> 8)&0xf) };

   cmd_write(LM5066_CMD_VIN_UV_WARN_LIMIT, data, 2);
}

double lm5066::get_undervoltage_warn_val() {

   uint8_t recved[2];

   cmd_read(LM5066_CMD_VIN_UV_WARN_LIMIT, recved, 2);

   uint16_t val = recved[0] | ((uint16_t)recved[1] << 8);

   return from_telemetry_int(val, 4617, -140, -2);
}

void lm5066::disable_undervoltage_warn() {

   uint8_t vals[2] = { 0xff, 0x0f };

   cmd_write(LM5066_CMD_VIN_UV_WARN_LIMIT,vals,2);

}

struct lm5066_ein_values {
   uint32_t accumulator;
   uint32_t sample_count;
};

double lm5066::get_ein() {

   uint8_t vals[7];

   cmd_read(LM5066_CMD_READ_EIN, vals, 7);

   curr_ein.accumulator = ((uint32_t)vals[3] << 15) + ((uint32_t)vals[2] << 8) + vals[1];
   curr_ein.sample_count = ((uint32_t)vals[6] << 16) + ((uint32_t)vals[5] << 8) + vals[4];

   /* adjust if rollover has occured */
   if (curr_ein.accumulator < prev_ein.accumulator)
      curr_ein.accumulator += 0x7fffff;

   /* adjust if rollover has occured */
   if (curr_ein.sample_count < prev_ein.sample_count)
      curr_ein.sample_count += 0xffffff;

   double res = ((double)(curr_ein.accumulator - prev_ein.accumulator)) / 
                ((double)(curr_ein.sample_count - prev_ein.sample_count));

   /* revert to before rollover to preserve continuity */
   prev_ein.accumulator = curr_ein.accumulator % 0x7fffff;
   prev_ein.sample_count = curr_ein.sample_count % 0xffffff;

   return res;
}

double lm5066::get_ein_safe() {

   get_ein();

   sleep_ms(50);

   return get_ein();
}

double lm5066::get_vin() {

   uint8_t recved[2];

   cmd_read(LM5066_CMD_READ_VIN, recved, 2);

   uint16_t val = ((uint16_t)recved[1] << 8) | recved[0];

   return from_telemetry_int(val, 4617, -140.0, -2);
}

double lm5066::get_iin() {

   uint8_t recved[2];

   cmd_read(LM5066_CMD_READ_IIN, recved, 2);

   uint16_t val = ((uint16_t)recved[1] << 8) | recved[0];

   return from_telemetry_int(val, (15076.0f * this->sense_resistor), -503.9, -2);
}


double lm5066::get_vout() {

   uint8_t recved[2];

   cmd_read(LM5066_CMD_READ_VOUT, recved, 2);

   uint16_t val = ((uint16_t)recved[1] << 8) | recved[0];

   return from_telemetry_int(val, 4602, -500, -2);
}

double lm5066::get_temperature() {

   uint8_t recved[2];

   cmd_read(LM5066_CMD_READ_TEMPERATURE_1, recved, 2);

   uint16_t val = ((uint16_t)recved[1] << 8) | recved[0];

   return from_telemetry_int(val, 16000, 0, -3);
}

double lm5066::get_pin() {

   uint8_t recved[2];

   cmd_read(LM5066_CMD_READ_PIN, recved, 2);

   uint16_t val = ((uint16_t)recved[1] << 8) | recved[0];

   return from_telemetry_int(val, (1701.0f * this->sense_resistor), -4000, -3);
}

void lm5066::get_part_string(char * buf, int size) {

   if (size < 14)
      return;

   uint8_t recved[9];

   cmd_read(LM5066_CMD_MFR_ID, recved, 4);

   *(buf+0) = recved[1];
   *(buf+1) = recved[2];
   *(buf+2) = ' ';

   cmd_read(LM5066_CMD_MFR_MODEL, recved, 9);

   *(buf+3) = recved[1];
   *(buf+4) = recved[2];
   *(buf+5) = recved[3];
   *(buf+6) = recved[4];
   *(buf+7) = recved[5];
   *(buf+8) = recved[6];
   *(buf+9) = recved[7];
   *(buf+10) = ' ';

   cmd_read(LM5066_CMD_MFR_REVISION, recved, 3);

   *(buf+11) = recved[1];
   *(buf+12) = recved[2];
   *(buf+13) = ' ';
   *(buf+14) = '\0';
}

double lm5066::get_vaux() {

   uint8_t recved[2];

   cmd_read(LM5066_CMD_READ_VAUX, recved, 2);

   uint16_t val = ((uint16_t)recved[1] << 8) | recved[0];

   return from_telemetry_int(val, 13774, 73, -1);
}

void lm5066::set_overcurrent_warn_val(double apms) {

   uint16_t to_send = to_telemetry_int(apms, (7645.0f * this->sense_resistor), 100, -2);
   uint8_t data[2] = { (uint8_t)(to_send & 0xff), (uint8_t)((to_send >> 8)&0xf) };

   cmd_write(LM5066_CMD_MFR_IIN_OC_WARN_LIMIT, data, 2);
}

double lm5066::get_overcurrent_warn_val() {

   uint8_t recved[2];

   cmd_read(LM5066_CMD_MFR_IIN_OC_WARN_LIMIT, recved, 2);

   uint16_t val = recved[0] | ((uint16_t)recved[1] << 8);

   return from_telemetry_int(val, (7645.0f * this->sense_resistor), 100, -2);
}

void lm5066::disable_overcurrent_warn() {

   uint8_t vals[2] = { 0xff, 0x0f };

   cmd_write(LM5066_CMD_MFR_IIN_OC_WARN_LIMIT,vals,2);
}

void lm5066::disable_overpower_warn() {

   uint8_t vals[2] = { 0xff, 0x0f };

   cmd_write(LM5066_CMD_MFR_PIN_OP_WARN_LIMIT,vals,2);
}

void lm5066::set_overpower_warn_val(double watts) {

   uint16_t to_send = to_telemetry_int(watts, (860.6f * this->sense_resistor), -965, -3);
   uint8_t data[2] = { (uint8_t)(to_send & 0xff), (uint8_t)((to_send >> 8)&0xf) };

   cmd_write(LM5066_CMD_MFR_PIN_OP_WARN_LIMIT, data, 2);
}

double lm5066::get_overpower_warn_val() {

   uint8_t recved[2];

   cmd_read(LM5066_CMD_MFR_PIN_OP_WARN_LIMIT, recved, 2);

   uint16_t val = recved[0] | ((uint16_t)recved[1] << 8);

   return from_telemetry_int(val, (860.6f * this->sense_resistor), -965, -3);
}

double lm5066::get_max_pin() {

   uint8_t recved[2];

   cmd_read(LM5066_CMD_READ_PIN_PEAK, recved, 2);

   uint16_t val = recved[0] | ((uint16_t)recved[1] << 8);

   return from_telemetry_int(val, (1701.0f * this->sense_resistor), -4000, -3);
}

void lm5066::clear_max_pin() {

   uint8_t send_byte = 0;

   cmd_write(LM5066_CMD_CLEAR_PIN_PEAK,&send_byte,1);
}

void lm5066::set_gate_mask(uint8_t mask, uint8_t * prev_mask) {

   if (prev_mask)
      cmd_read(LM5066_CMD_GATE_MASK, prev_mask, 1);

   cmd_write(LM5066_CMD_GATE_MASK, &mask, 1);
}

void lm5066::set_alert_mask(uint16_t mask, uint16_t * prev_mask) {

   if (prev_mask) {
      uint8_t res[2];
      cmd_read(LM5066_CMD_ALERT_MASK, res, 2);

      *prev_mask = res[0] | ((uint16_t)res[1] << 8);
   }

   uint8_t send[2] = { (uint8_t)(mask & 0xff), (uint8_t)(mask >> 8) };

   cmd_write(LM5066_CMD_ALERT_MASK, send, 2);
}

void lm5066::set_device_config(enum lm5066_retry_count retries, enum lm5066_cl_setting cl_setting,
                               enum lm5066_cb_cl_ratio ratio  , enum lm5066_cl_config  cl_config  ) {

   uint8_t val = ((retries    & 0x7) << 5) |
                 ((cl_setting & 0x1) << 4) |
                 ((ratio      & 0x1) << 3) |
                 ((cl_config  & 0x1) << 2);

   cmd_write(LM5066_CMD_DEVICE_SETUP, &val, 1);

}

void lm5066::update_diagnostics() {

   uint8_t recved[2];

   cmd_read(LM5066_CMD_DIAGNOSTIC_WORD_READ, recved, 2);

   diagnostic_word = (recved[0]) | ((uint16_t)recved[1] << 8);
}

void lm5066::set_samples_average(uint8_t two_to_n) {

   if (two_to_n > 0b1100)
      two_to_n = 0b1100;

   cmd_write(LM5066_CMD_SAMPLES_FOR_AVG, &two_to_n, 1);
}

double lm5066::get_avg_vin() {

   uint8_t recved[2];

   cmd_read(LM5066_CMD_READ_AVG_VIN, recved, 2);

   uint16_t val = ((uint16_t)recved[1] << 8) | recved[0];

   avg_vin =  from_telemetry_int(val, 4617, -140.0, -2);

   return avg_vin;
}

double lm5066::get_avg_vout() {

   uint8_t recved[2];

   cmd_read(LM5066_CMD_READ_AVG_VOUT, recved, 2);

   uint16_t val = ((uint16_t)recved[1] << 8) | recved[0];

   avg_vout =  from_telemetry_int(val, 4602, -500, -2);

   return avg_vout;
}

double lm5066::get_avg_iin() {

   uint8_t recved[2];

   cmd_read(LM5066_CMD_READ_AVG_IIN, recved, 2);

   uint16_t val = ((uint16_t)recved[1] << 8) | recved[0];

   avg_iin = from_telemetry_int(val, (15076.0f * this->sense_resistor), -503.9, -2);

   return avg_iin;
}

double lm5066::get_avg_pin() {

   uint8_t recved[2];

   cmd_read(LM5066_CMD_READ_AVG_PIN, recved, 2);

   uint16_t val = ((uint16_t)recved[1] << 8) | recved[0];

   avg_pin = from_telemetry_int(val, (1701.0f * this->sense_resistor), -4000, -3);

   return avg_pin;
}

uint16_t lm5066::get_black_box() {

   uint8_t recved[2];

   cmd_read(LM5066_CMD_BLACK_BOX_READ, recved, 2);

   uint16_t val = ((uint16_t)recved[1] << 8) | recved[0];

   return val;
}

void lm5066::read_values() {

   uint8_t recved[13];

   cmd_read(LM5066_CMD_AVG_BLOCK_READ, recved, 13);

   diagnostic_word = ((uint16_t)recved[2] << 8)  | recved[1];
   avg_iin         = ((uint16_t)recved[4] << 8)  | recved[3];
   avg_vout        = ((uint16_t)recved[6] << 8)  | recved[5];
   avg_vin         = ((uint16_t)recved[8] << 8)  | recved[7];
   avg_pin         = ((uint16_t)recved[10] << 8) | recved[9];
   temperature     = ((uint16_t)recved[12] << 8) | recved[11];
}


bool lm5066::vout_uv_warn() {
   return (diagnostic_word & LM5066_DIAGNOSTIC_VOUT_UNDERVOLTAGE_WARN ? true : false);
}


bool lm5066::iin_op_warn() {
   return (diagnostic_word & LM5066_DIAGNOSTIC_IIN_OP_WARN ? true : false);
}


bool lm5066::vin_uv_warn() {
   return (diagnostic_word & LM5066_DIAGNOSTIC_VIN_UNDERVOLTAGE_WARN ? true : false);
}


bool lm5066::vin_ov_warn() {
   return (diagnostic_word & LM5066_DIAGNOSTIC_VIN_OVERVOLTAGE_WARN ? true : false);
}


bool lm5066::power_good() {
   return (diagnostic_word & LM5066_DIAGNOSTIC_POWER_GOOD_INVERTED ? true : false);
}


bool lm5066::ot_warn() {
   return (diagnostic_word & LM5066_DIAGNOSTIC_OVERTEMPERATURE_WARN ? true : false);
}


bool lm5066::timer_latched() {
   return (diagnostic_word & LM5066_DIAGNOSTIC_TIMER_LATCHED ? true : false);
}


bool lm5066::mosfet_shorted() {
   return (diagnostic_word & LM5066_DIAGNOSTIC_MOSFET_SHORTED ? true : false);
}


bool lm5066::config_preset() {
   return (diagnostic_word & LM5066_DIAGNOSTIC_CONFIG_PRESET ? true : false);
}


bool lm5066::device_off() {
   return (diagnostic_word & LM5066_DIAGNOSTIC_DEVICE_OFF ? true : false);
}


bool lm5066::vin_uv_fault() {
   return (diagnostic_word & LM5066_DIAGNOSTIC_VIN_UNDERVOLTAGE_FAULT ? true : false);
}


bool lm5066::vin_ov_fault() {
   return (diagnostic_word & LM5066_DIAGNOSTIC_VIN_OVERVOLTAGE_FAULT ? true : false);
}


bool lm5066::iin_fault() {
   return (diagnostic_word & LM5066_DIAGNOSTIC_OVERCURRENT_FAULT ? true : false);
}


bool lm5066::temp_fault() {
   return (diagnostic_word & LM5066_DIAGNOSTIC_TEMPERATURE_FAULT ? true : false);
}


bool lm5066::cml_fault() {
   return (diagnostic_word & LM5066_DIAGNOSTIC_CML_FAULT ? true : false);
}


bool lm5066::cb_fault() {
   return (diagnostic_word & LM5066_DIAGNOSTIC_CIRCUIT_BREAKER_FAULT ? true : false);
}

