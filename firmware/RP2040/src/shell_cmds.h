
#ifndef __RP2040_FIRMWARE_SRC_SHELL_CMDS__

#define __RP2040_FIRMWARE_SRC_SHELL_CMDS__
#include "shell.h"

/* shell base commands */
int cd(void * arg, shell * sh);
int ls(void * arg, shell * sh);
int cat(void * arg, shell * sh);
int echo(void * arg, shell * sh);
int credits(void * arg, shell * sh);
int megumin(void * arg, shell * sh);
int lr(void * arg, shell * sh);
int errno(void * arg, shell * sh);
int clear(void * arg, shell * sh);
int help(void * arg, shell * sh);
int status(void * arg, shell * sh);
int set(void * arg, shell * sh);
int get(void * arg, shell * sh);

#define ADD_SHELL_BASE_COMMANDS(sh)                                 \
   ADD_SHELL_COMMAND(cd,SHELL_DEVICE_ANY,SHELL_INPUT_STRING,sh);    \
   ADD_SHELL_COMMAND(ls,SHELL_DEVICE_ANY,SHELL_INPUT_NONE,sh);      \
   ADD_SHELL_COMMAND(cat,SHELL_DEVICE_ANY,SHELL_INPUT_STRING,sh);   \
   ADD_SHELL_COMMAND(echo,SHELL_DEVICE_ANY,SHELL_INPUT_STRING,sh);  \
   ADD_SHELL_COMMAND(credits,SHELL_DEVICE_ANY,SHELL_INPUT_NONE,sh); \
   ADD_SHELL_COMMAND(lr,SHELL_DEVICE_ANY,SHELL_INPUT_NONE,sh);      \
   ADD_SHELL_COMMAND(help,SHELL_DEVICE_ANY,SHELL_INPUT_NONE,sh);    \
   ADD_SHELL_COMMAND(errno,SHELL_DEVICE_ANY,SHELL_INPUT_NONE,sh);   \
   ADD_SHELL_COMMAND(clear,SHELL_DEVICE_ANY,SHELL_INPUT_NONE,sh);   \
   ADD_SHELL_COMMAND(status,SHELL_DEVICE_ANY,SHELL_INPUT_STRING,sh);\
   ADD_SHELL_COMMAND(set,SHELL_DEVICE_ANY,SHELL_INPUT_STR_NUM,sh);  \
   ADD_SHELL_COMMAND(get,SHELL_DEVICE_ANY,SHELL_INPUT_STRING,sh);   \
   ADD_SHELL_COMMAND(megumin,SHELL_DEVICE_ANY,SHELL_INPUT_NONE,sh)

/* shell lm5066 commands */

#define ADD_SHELL_LM5066_COMMANDS(sh) \
   do { \
   } while (0)

/* shell led1642gw commands */
int ledon(void * arg, shell * sh);
int ledof(void * arg, shell * sh);

#define ADD_SHELL_LED1642GW_COMMANDS(sh) \
   ADD_SHELL_COMMAND(ledon,SHELL_DEVICE_LED1642GW,SHELL_INPUT_NUMBER,sh); \
   ADD_SHELL_COMMAND(ledoff,SHELL_DEVICE_LED1642GW,SHELL_INPUT_NUMBER,sh)


/* shell ads1015 commands */

#define ADD_SHELL_ADS1015_COMMANDS(sh) \
   do { \
   } while (0)

#endif
