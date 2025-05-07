
#include "lm5066.h"
#include <limits.h>

lm5066::lm5066(i2c_inst_t * i2c_conn, uint8_t addr, uint32_t smba_pin) {
   this->i2c_conn = i2c_conn;
   this->addr = addr;
   this->smba_pin = smba_pin;
}

void lm5066::send_command(uint8_t cmd, bool rw) {

   i2c_write_byte_raw(i2c_conn, ((addr << 1) | rw));

   i2c_write_byte_raw(i2c_conn, cmd);

}

void lm5066::write_data(uint8_t * bytes, int num) {

   for (int i = 0; i < num; ++i) {

      i2c_write_byte_raw(i2c_conn, bytes[i]);
      
   }

}

void lm5066::read_data(uint8_t * bytes, int num) {

   for (int i = 0; i < num; ++i) {

      bytes[i] = i2c_read_byte_raw(i2c_conn);
      
   }

}

void lm5066::cmd_read(uint8_t cmd, uint8_t * bytes, int num) {

   send_command(cmd,false);

   read_data(bytes,num);

}

void lm5066::cmd_write(uint8_t cmd, uint8_t * bytes, int num) {

   send_command(cmd,true);

   write_data(bytes,num);

}

void lm5066::reenable() {

   set_mosfet(false);

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

   send_command(LM5066_CMD_CLEAR_FAULTS,1);

}

void lm5066::disable_vout_undervoltage_warn() {

   uint8_t vals[2] = { 0x00, 0x00 };

   cmd_write(LM5066_CMD_VOUT_UV_WARN_LIMIT,vals,2);

}

uint16_t lm5066::to_telemetry_int(double val, double m, double b, int R) {

   uint32_t res = (((val * m) + b) / exp10((double)(-R)));

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
   uint8_t data[2] = { (uint8_t)(to_send & 0xff), (uint8_t)(to_send >> 8) };

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
   uint8_t data[2] = { (uint8_t)(to_send & 0xff), (uint8_t)(to_send >> 8) };

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
   uint8_t data[2] = { (uint8_t)(to_send & 0xff), (uint8_t)(to_send >> 8) };

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
   uint8_t data[2] = { (uint8_t)(to_send & 0xff), (uint8_t)(to_send >> 8) };

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
   uint8_t data[2] = { (uint8_t)(to_send & 0xff), (uint8_t)(to_send >> 8) };

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

   return from_telemetry_int(val, 15076, -503.9, -2);
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

   return from_telemetry_int(val, 1701, -4000, -3);
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

   uint16_t to_send = to_telemetry_int(apms, 7645, 100, -2);
   uint8_t data[2] = { (uint8_t)(to_send & 0xff), (uint8_t)(to_send >> 8) };

   cmd_write(LM5066_CMD_MFR_IIN_OC_WARN_LIMIT, data, 2);
}

double lm5066::get_overcurrent_warn_val() {

   uint8_t recved[2];

   cmd_read(LM5066_CMD_MFR_IIN_OC_WARN_LIMIT, recved, 2);

   uint16_t val = recved[0] | ((uint16_t)recved[1] << 8);

   return from_telemetry_int(val, 7645, 100, -2);
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

   uint16_t to_send = to_telemetry_int(watts, 860.6, -965, -3);
   uint8_t data[2] = { (uint8_t)(to_send & 0xff), (uint8_t)(to_send >> 8) };

   cmd_write(LM5066_CMD_MFR_PIN_OP_WARN_LIMIT, data, 2);
}

double lm5066::get_overpower_warn_val() {

   uint8_t recved[2];

   cmd_read(LM5066_CMD_MFR_PIN_OP_WARN_LIMIT, recved, 2);

   uint16_t val = recved[0] | ((uint16_t)recved[1] << 8);

   return from_telemetry_int(val, 860.6, -965, -3);
}

