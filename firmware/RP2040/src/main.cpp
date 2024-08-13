#include "board.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pico/stdlib.h>
#include <pico/binary_info.h>
#include <hardware/gpio.h>
#include <hardware/pwm.h>
#include <hardware/adc.h>
#include <hardware/irq.h>
#include <hardware/i2c.h>

#include "gpio_utils.h"
#include "version.h"
#include "flags.h"


char FIRMWARE_VERSION[128];


void setup(void) {
    stdio_init_all();

    #ifdef USB_DEBUG
    while (!stdio_usb_connected());
    #endif
    
    setup_default_uart();

    // Print the firmware version
    strncat(FIRMWARE_VERSION, GIT_REV, 16);
    strncat(FIRMWARE_VERSION, "_", 2);
    strncat(FIRMWARE_VERSION, GIT_TAG, 64);
    strncat(FIRMWARE_VERSION, "_", 2);
    strncat(FIRMWARE_VERSION, GIT_BRANCH, 32);
    printf("System starting up\n");
    printf("Chip version: %u\n", rp2040_chip_version());
    printf("ROM version: %u\n", rp2040_rom_version());
    printf("Firmware version: %s\n", FIRMWARE_VERSION);
    printf("\n");

    // Initialize the GPIO pins
    adc_init();
    PWM_set_wrap(PWM_WRAP);

    gpio_init(LED_CLK, OUTPUT);
    gpio_init(LED_SDO, OUTPUT);

    gpio_init(EN_B1, OUTPUT);
    gpio_init(EN_B2, OUTPUT);
    gpio_init(EN_B3, OUTPUT);

    gpio_init(PGD_B1, INPUT);
    gpio_init(PGD_B2, INPUT);
    gpio_init(PGD_B3, INPUT);

    gpio_init(ALRT_B1, INPUT);
    gpio_init(ALRT_B2, INPUT);
    gpio_init(ALRT_B3, INPUT);

    gpio_init(ADC1_RDY, INPUT);
    gpio_init(ADC2_RDY, INPUT);

    gpio_init(EN_AUX1, OUTPUT);
    gpio_init(EN_AUX2, OUTPUT);
    gpio_init(EN_AUX3, OUTPUT);

    gpio_init(PGD_AUX1, INPUT);
    gpio_init(PGD_AUX2, INPUT);
    gpio_init(PGD_AUX3, INPUT);

    // Initialize the I2C pins
    gpio_set_function(I2C0_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C0_SCL, GPIO_FUNC_I2C);

    gpio_set_function(I2C1_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C1_SCL, GPIO_FUNC_I2C);

    // bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));
}

int main() {
    setup();
    return 0;
}