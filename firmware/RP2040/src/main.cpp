#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "hardware/timer.h"

#include "board.h"
#include "version.h"
#include "shell.h"
#include "shell_cmds.h"

#include "../lib/LM5066/lm5066.h"
#include "../lib/ADS1015/ads1015.h"
#include "../lib/LED1642GW/led1642gw.h"


enum LED_NUMBERS {
    PWR_ON        = 0,
    PGD           = 1,
    STATUS_1      = 2,
    STATUS_2      = 3,
    STATUS_3      = 4,
    STATUS_4      = 5,
    STATUS_5      = 6,
    STATUS_6      = 7,
    STATUS_7      = 8,
    STATUS_8      = 9,
    ERR_BATTERY_1 = 10,
    ERR_BATTERY_2 = 11,
    ERR_BATTERY_3 = 12,
    ERR_AUX_1     = 13,
    ERR_AUX_2     = 14,
    ERR_AUX_3     = 15
};


int setup() {
   printf("Initializing stdio...\n");
   stdio_init_all();

   // Initialize uart0
   printf("Initializing UART0...\n");
   uart_init(uart0, 115200);
   gpio_set_function(UART_0_TX, GPIO_FUNC_UART); // TX
   gpio_set_function(UART_0_RX, GPIO_FUNC_UART); // RX

   // Initialize i2c0
   printf("Initializing I2C0...\n");
   i2c_init(i2c0, 100 * 1000); // 100 kHz
   gpio_set_function(I2C0_SDA, GPIO_FUNC_I2C); // SDA
   gpio_set_function(I2C0_SCL, GPIO_FUNC_I2C); // SCL
   gpio_pull_up(I2C0_SDA);
   gpio_pull_up(I2C0_SCL);

   // Initialize i2c1
   printf("Initializing I2C1...\n");
   i2c_init(i2c1, 100 * 1000); // 100 kHz
   gpio_set_function(I2C1_SDA, GPIO_FUNC_I2C); // SDA
   gpio_set_function(I2C1_SCL, GPIO_FUNC_I2C); // SCL
   gpio_pull_up(I2C1_SDA);
   gpio_pull_up(I2C1_SCL);

   // Initialize GPIO for LED driver
   printf("Initializing SPI for LED driver...\n");
   gpio_set_dir(LED_CLK, GPIO_OUT); // CLK (MCU out)
   gpio_put(LED_CLK, 0); // Set CLK to low

   gpio_set_dir(LED_SDI, GPIO_OUT); // SDI (MCU out)
   gpio_put(LED_SDI, 0); // Set SDI to low

   gpio_set_dir(LED_LEN, GPIO_OUT); // Latch Enable
   gpio_put(LED_LEN, 0);


   // Initialize GPIO for battery control
   printf("Initializing GPIO for battery control...\n");
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
   printf("Initializing GPIO for ADC...\n");
   gpio_init(ADC_1_RDY);
   gpio_set_dir(ADC_1_RDY, GPIO_IN);

   gpio_init(ADC_2_RDY);
   gpio_set_dir(ADC_2_RDY, GPIO_IN);

   // Initialize GPIO for auxiliary output modules
   printf("Initializing GPIO for auxiliary output modules...\n");
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

   printf("Setup complete.\n");
   return 0;
}

void led_startup(LED1642GW &led_driver) {
   led_driver.setLedOn(PWR_ON);

   for (int i = STATUS_1; i <= STATUS_8; i++) {
      led_driver.setLedOn(i);
      sleep_ms(50);
   }

   for (int i = ERR_BATTERY_1; i <= ERR_AUX_3; i++) {
      led_driver.setLedOn(i);
      sleep_ms(50);
   }

   sleep_ms(250);

   for (int i = ERR_AUX_3; i >= ERR_BATTERY_1; i--) {
      led_driver.setLedOff(i);
      sleep_ms(50);
   }

   for (int i = STATUS_8; i >= STATUS_1; i--) {
      led_driver.setLedOff(i);
      sleep_ms(50);
   }
}

int main() {

   // Wait for USB serial connection before proceeding
   printf("Waiting for USB serial connection...\n");
   sleep_ms(5000);
   printf("USB serial connected.\n");

   printf("Starting main()...\n");
   // Initialize the board
   printf("Calling setup()...\n");
   setup();
   printf("Returned from setup(). Initializing peripherals...\n");

   lm5066 battery1 = lm5066(i2c0, BATTERY_1_ADDR, BATTERY_1_ALERT, 500e-6f);
   lm5066 battery2 = lm5066(i2c0, BATTERY_2_ADDR, BATTERY_2_ALERT, 500e-6f);
   lm5066 battery3 = lm5066(i2c0, BATTERY_3_ADDR, BATTERY_3_ALERT, 500e-6f);

   LED1642GW led_driver = LED1642GW(LED_CLK, LED_SDI, LED_LEN);
   led_driver.begin();

   ads1015 adc1 = ads1015(i2c0, ADC_1_ADDR, ADC_1_RDY);
   ads1015 adc2 = ads1015(i2c0, ADC_2_ADDR, ADC_2_RDY);

   lm5066 aux1 = lm5066(i2c1, AUX_1_ADDR, AUX_DUMMY_ALERT, 1.5e-3f);
   lm5066 aux2 = lm5066(i2c1, AUX_2_ADDR, AUX_DUMMY_ALERT, 1.5e-3f);
   lm5066 aux3 = lm5066(i2c1, AUX_3_ADDR, AUX_DUMMY_ALERT, 1.5e-3f);

   led_startup(led_driver);

   printf("Initializing shell...\n");
   shell sh;

   /* add shell devices */
   ADD_SHELL_DEVICE_LM5066(battery1,sh);
   ADD_SHELL_DEVICE_LM5066(battery2,sh);
   ADD_SHELL_DEVICE_LM5066(battery3,sh);
   ADD_SHELL_DEVICE_LM5066(aux1,sh);
   ADD_SHELL_DEVICE_LM5066(aux2,sh);
   ADD_SHELL_DEVICE_LM5066(aux3,sh);
   ADD_SHELL_DEVICE_LED1642GW(led_driver,sh);
   ADD_SHELL_DEVICE_ADS1015(adc1,sh);
   ADD_SHELL_DEVICE_ADS1015(adc2,sh);

   /* add shell commands */
   ADD_SHELL_BASE_COMMANDS(sh);
   ADD_SHELL_LM5066_COMMANDS(sh);
   ADD_SHELL_LED1642GW_COMMANDS(sh);
   ADD_SHELL_ADS1015_COMMANDS(sh);

   printf("Entering main loop...\n");
   while (1) {

      sleep_ms(50);

      sh.poll_input();
   }

}
