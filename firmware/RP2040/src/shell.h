
#ifndef __RP2040_FIRMWARE_SRC_SHELL__

#define __RP2040_FIRMWARE_SRC_SHELL__

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"

#include "ads1015.h"
#include "led1642gw.h"
#include "lm5066.h"


/* no argument is passed to this command */
#define SHELL_CMD_ARG_NONE 0b00
/* an int is passed to this command */
#define SHELL_CMD_ARG_INT  0b01
/* a double is passed to this command */
#define SHELL_CMD_ARG_DBL  0b10
/* a string is passed to this command */
#define SHELL_CMD_ARG_STR  0b10

/* how long is each line? */
#define SHELL_LINE_LENGTH 36
/* how many times can we press the 
 * up arrow to go back?           */
#define SHELL_SCROLLBACK 5

/* maximum number of devices allowed in the shell */
#define SHELL_DEVICE_COUNT 8

/* wrapper */
#define SHELL_GETCHAR() stdio_getchar_timeout_us(0)

const uint8_t SHELL_UP_KEY   [3] = { 27, 91, 65 };
const uint8_t SHELL_DOWN_KEY [3] = { 27, 91, 66 };
const uint8_t SHELL_RIGHT_KEY[3] = { 27, 91, 67 };
const uint8_t SHELL_LEFT_KEY [3] = { 27, 91, 68 };
const uint8_t SHELL_BACKSPACE_0 = 0x08;
const uint8_t SHELL_BACKSPACE_1 = 0x7f;
const uint8_t SHELL_TAB         = 0x09;
const uint8_t SHELL_ENTER       = 0x0a;

const uint8_t SHELL_INPUT_DONE = PICO_ERROR_TIMEOUT;

/* a buffer that writes over previous values 
 * if you go forward over it's limit.       */
struct circular_buffer {
   uint16_t pos;
   char buf[SHELL_SCROLLBACK][SHELL_LINE_LENGTH];
};

void circular_buffer_add(char * val, struct circular_buffer * buf);
void circular_buffer_get(char * val, uint8_t pos, struct circular_buffer * buf);

#define SHELL_DEVICE_NONE      0b00
#define SHELL_DEVICE_ADS1015   0b01
#define SHELL_DEVICE_LED1642GW 0b10
#define SHELL_DEVICE_LM5066    0b10

#define SHELL_INPUT_NONE      0b00
#define SHELL_INPUT_NUMBER    0b01
#define SHELL_INPUT_STRING    0b10

/* a device that the shell should be able to 
 * interface with. Will show up as a directory */
struct shell_device {
   uint8_t device_type;
   char name[SHELL_LINE_LENGTH];
   union {
      lm5066    * lm;
      ads1015   * ads;
      LED1642GW * led;
   };
};

/* a command that the shell can execute. Note that
 * some commands are tied to the specific devices
 * we are in. This breaks the unix-likeness a bit
 * but I don't care - we aren't unix here!       */
struct shell_command {
   uint8_t device_type;
   uint8_t input_type;
   char name[SHELL_LINE_LENGTH];

   /* note: all commands are passed the parsed input
    * as defined by the input_type above, as well as 
    * a pointer to the current shell to make any
    * needed modifications or read operations.     */
   int (* command )(void *, class shell *);

   struct shell_command * next;
};

class shell {
public:

/* quasi-private */

   uint8_t scrollback;
   uint8_t linepos;
   uint8_t linemax;
   uint8_t device_count;
   uint8_t lastres;
   uint8_t typed;

   struct circular_buffer scrollback_buf;
   struct shell_device * devices[SHELL_DEVICE_COUNT];
   struct shell_device * dir;
   struct shell_command * commands;

   char buf[SHELL_LINE_LENGTH];

   struct shell_command * print_partial_matches();
   int execute_command();
   void delete_character();

/* intended to be used by the user */

   /* initialize all needed io and variables */
   shell();

   void add_command(struct shell_command * command);
   void add_device(struct shell_device * device);
   void poll_input();

};

#endif
