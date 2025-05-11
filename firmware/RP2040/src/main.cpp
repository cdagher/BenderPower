#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "hardware/timer.h"

#include "board.h"
#include "version.h"

#include "../lib/LM5066/lm5066.h"
#include "../lib/ADS1015/ads1015.h"
#include "../lib/LED1642GW/led1642gw.h"


lm5066 battery1;
lm5066 battery2;
lm5066 battery3;

LED1642GW led_driver;

ads1015 adc1;
ads1015 adc2;

lm5066 aux1;
lm5066 aux2;
lm5066 aux3;


int setup();


int main() {

   // Initialize the board
   setup();

   battery1 = lm5066(i2c0, BATTERY_1_ADDR, BATTERY_1_ALERT, 500e-6f);
   battery2 = lm5066(i2c0, BATTERY_2_ADDR, BATTERY_2_ALERT, 500e-6f);
   battery3 = lm5066(i2c0, BATTERY_3_ADDR, BATTERY_3_ALERT, 500e-6f);

   led_driver = LED1642GW(spi0, LED_LEN);

   adc1 = ads1015(i2c0, ADC_1_ADDR, ADC_1_RDY);
   adc2 = ads1015(i2c0, ADC_2_ADDR, ADC_2_RDY);

   aux1 = lm5066(i2c1, AUX_1_ADDR, AUX_DUMMY_ALERT, 1.5e-3f);
   aux2 = lm5066(i2c1, AUX_2_ADDR, AUX_DUMMY_ALERT, 1.5e-3f);
   aux3 = lm5066(i2c1, AUX_3_ADDR, AUX_DUMMY_ALERT, 1.5e-3f);

   while (1) {

      sleep_ms(10000);

   }

}

int setup() {
   stdio_init_all();

   // Initialize uart0
   uart_init(uart0, 115200);
   gpio_set_function(UART_0_TX, GPIO_FUNC_UART); // TX
   gpio_set_function(UART_0_RX, GPIO_FUNC_UART); // RX

   // Initialize i2c0
   i2c_init(i2c0, 100 * 1000); // 100 kHz
   gpio_set_function(I2C0_SDA, GPIO_FUNC_I2C); // SDA
   gpio_set_function(I2C0_SCL, GPIO_FUNC_I2C); // SCL
   gpio_pull_up(I2C0_SDA);
   gpio_pull_up(I2C0_SCL);

   // Initialize i2c1
   i2c_init(i2c1, 100 * 1000); // 100 kHz
   gpio_set_function(I2C1_SDA, GPIO_FUNC_I2C); // SDA
   gpio_set_function(I2C1_SCL, GPIO_FUNC_I2C); // SCL
   gpio_pull_up(I2C1_SDA);
   gpio_pull_up(I2C1_SCL);

   // Initialize SPI for LED driver
   spi_init(spi0, 100 * 1000); // 100 kHz
   gpio_set_function(LED_CLK, LED_CLK_MODE); // CLK
   gpio_set_function(LED_SDI, LED_SDI_MODE); // SDI
   gpio_set_dir(LED_LEN, GPIO_OUT); // Latch Enable
   gpio_put(LED_LEN, 0);


   // Initialize GPIO for battery control
   gpio_init(BATTERY_1_EN);
   gpio_set_dir(BATTERY_1_EN, GPIO_OUT);

   gpio_init(BATTERY_1_ALERT);
   gpio_set_dir(BATTERY_1_ALERT, GPIO_IN);
   gpio_pull_up(BATTERY_1_ALERT);

   gpio_init(BATTERY_1_PGD);
   gpio_set_dir(BATTERY_1_PGD, GPIO_IN);
   gpio_pull_up(BATTERY_1_PGD);

   gpio_init(BATTERY_2_EN);
   gpio_set_dir(BATTERY_2_EN, GPIO_OUT);

   gpio_init(BATTERY_2_ALERT);
   gpio_set_dir(BATTERY_2_ALERT, GPIO_IN);
   gpio_pull_up(BATTERY_2_ALERT);

   gpio_init(BATTERY_2_PGD);
   gpio_set_dir(BATTERY_2_PGD, GPIO_IN);
   gpio_pull_up(BATTERY_2_PGD);

   gpio_init(BATTERY_3_EN);
   gpio_set_dir(BATTERY_3_EN, GPIO_OUT);

   gpio_init(BATTERY_3_ALERT);
   gpio_set_dir(BATTERY_3_ALERT, GPIO_IN);
   gpio_pull_up(BATTERY_3_ALERT);

   gpio_init(BATTERY_3_PGD);
   gpio_set_dir(BATTERY_3_PGD, GPIO_IN);
   gpio_pull_up(BATTERY_3_PGD);

   // Initialize GPIO for ADC
   gpio_init(ADC_1_RDY);
   gpio_set_dir(ADC_1_RDY, GPIO_IN);

   gpio_init(ADC_2_RDY);
   gpio_set_dir(ADC_2_RDY, GPIO_IN);

   // Initialize GPIO for auxiliary output modules
   gpio_init(AUX_DUMMY_ALERT);
   gpio_set_dir(AUX_DUMMY_ALERT, GPIO_IN);
   gpio_pull_up(AUX_DUMMY_ALERT);

   gpio_init(AUX_1_EN);
   gpio_set_dir(AUX_1_EN, GPIO_OUT);
   gpio_pull_up(AUX_1_EN);

   gpio_init(AUX_2_EN);
   gpio_set_dir(AUX_2_EN, GPIO_OUT);
   gpio_pull_up(AUX_2_EN);

   gpio_init(AUX_3_EN);
   gpio_set_dir(AUX_3_EN, GPIO_OUT);
   gpio_pull_up(AUX_3_EN);

   return 0;
}
