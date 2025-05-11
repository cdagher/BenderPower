// Board configuration for RP2040

#ifndef BOARD_H
#define BOARD_H

#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define BOARD_BENDER_BRAIN

// General pin configurations
#define UART_0_TX 0
#define UART_0_PIN_MODE GPIO_FUNC_UART
#define UART_0_RX 1
#define UART_0_PIN_MODE GPIO_FUNC_UART

#define I2C0_SDA 4
#define I2C0_PIN_MODE GPIO_FUNC_I2C
#define I2C0_SCL 5
#define I2C0_PIN_MODE GPIO_FUNC_I2C

#define I2C1_SDA 26
#define I2C1_PIN_MODE GPIO_FUNC_I2C
#define I2C1_SCL 27
#define I2C1_PIN_MODE GPIO_FUNC_I2C

// LED Driver pins
/*
LED Deiver is the LED1642GW from ST Microelectronics https://www.st.com/resource/en/datasheet/led1642gw.pdf

Note that the SDO pin is not used in this configuration, so there is no feedback
from the LED driver to the RP2040.
*/
#define LED_CLK 2
#define LED_CLK_MODE GPIO_FUNC_SIO
#define LED_SDI 3
#define LED_SDI_MODE GPIO_FUNC_SIO
#define LED_LEN  6
#define LED_LEN_MODE GPIO_FUNC_SIO

/* Battery Inputs 
Batteries are controlled using LM5066I ICs from Texas Instruments.

Batteries should use I2C0 for communication with the RP2040.
*/

#define BATTERY_1_ADDR 0x40
#define BATTERY_1_EN 12
#define BATTERY_1_EN_PIN_MODE GPIO_FUNC_SIO
#define BATTERY_1_ALERT 13
#define BATTERY_1_ALERT_PIN_MODE GPIO_FUNC_SIO
#define BATTERY_1_PGD 11
#define BATTERY_1_PGD_PIN_MODE GPIO_FUNC_SIO

#define BATTERY_2_ADDR 0x41
#define BATTERY_2_EN 10
#define BATTERY_2_EN_PIN_MODE GPIO_FUNC_SIO
#define BATTERY_2_ALERT 14
#define BATTERY_2_ALERT_PIN_MODE GPIO_FUNC_SIO
#define BATTERY_2_PGD 9
#define BATTERY_2_PGD_PIN_MODE GPIO_FUNC_SIO

#define BATTERY_3_ADDR 0x42
#define BATTERY_3_EN 8
#define BATTERY_3_EN_PIN_MODE GPIO_FUNC_SIO
#define BATTERY_3_ALERT 15
#define BATTERY_3_ALERT_PIN_MODE GPIO_FUNC_SIO
#define BATTERY_3_PGD 7
#define BATTERY_3_PGD_PIN_MODE GPIO_FUNC_SIO

/* ADC Inputs
ADCs are used to read the voltage output from the baord's auxiliary outputs.
The ADCs used are 2x ADS1015 from Texas Instruments

ADCs should use I2C1 for communication with the RP2040.
*/

#define ADC_1_ADDR 0x48
#define ADC_1_RDY 18
#define ADC_1_RDY_PIN_MODE GPIO_FUNC_SIO

#define ADC_2_ADDR 0x49
#define ADC_2_RDY 19
#define ADC_2_RDY_PIN_MODE GPIO_FUNC_SIO

/* Auxiliary Outputs
Auxiliary outputs are controlled using LM5066I ICs from Texas Instruments.
Note that due to a shortage of GPIO pins and board space, the auxiliary outputs
have no alert pins. This means that in order to detect a fault, the RP2040 must
poll the auxiliary outputs.

Auxiliary outputs should use I2C1 for communication with the RP2040.
*/

#define AUX_DUMMY_ALERT 16
#define AUX_DUMMY_ALERT_PIN_MODE GPIO_FUNC_SIO

#define AUX_1_ADDR 0x43
#define AUX_1_EN 21
#define AUX_1_EN_PIN_MODE GPIO_FUNC_SIO
// #define AUX_1_ALERT 20
#define AUX_1_PGD 20
#define AUX_1_PGD_PIN_MODE GPIO_FUNC_SIO

#define AUX_2_ADDR 0x44
#define AUX_2_EN 23
#define AUX_2_EN_PIN_MODE GPIO_FUNC_SIO
// #define AUX_2_ALERT 22
#define AUX_2_PGD 22
#define AUX_2_PGD_PIN_MODE GPIO_FUNC_SIO

#define AUX_3_ADDR 0x45
#define AUX_3_EN 25
#define AUX_3_EN_PIN_MODE GPIO_FUNC_SIO
// #define AUX_3_ALERT 24
#define AUX_3_PGD 24
#define AUX_3_PGD_PIN_MODE GPIO_FUNC_SIO

#endif // BOARD_H
