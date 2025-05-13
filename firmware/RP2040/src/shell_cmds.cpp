
#include "shell_cmds.h"
#include "shell.h"

#define STREQL(str1, str2) (0 == strcmp(str1,str2))

/* enter and exit the various devices attached to the board */
int cd(void * arg, shell * sh) {
   if (arg == NULL || *((char *)arg) == '\0') {
      printf("usage: cd [dir]\n");
      return 0;
   }

   if (STREQL((char *)arg,"..") || STREQL((char *)arg,"../")) {
      if (sh->dir == NULL) {
         printf("already in root directory\n");
         return 2;
      }
      else {
         sh->dir = NULL;
         return 0;
      }
   }

   if (STREQL((char *)arg,"~")) {
      sh->dir = NULL;
      return 0;
   }

   if (STREQL((char *)arg,"/")) {
      sh->dir = NULL;
      return 0;
   }

   if (STREQL((char *)arg,"."))
      return 0;

   if (STREQL((char *)arg,"./"))
      return 0;

   if (sh->dir == NULL) {

      char buf[SHELL_LINE_LENGTH+8];


      for (int i = 0; i < sh->device_count; ++i) {

         if (STREQL((char *)arg,sh->devices[i]->name)) {
            sh->dir = sh->devices[i];
            return 0;
         }

         memset(buf,0,sizeof(buf));
         strcpy(buf,"./");
         strcat(buf,sh->devices[i]->name);

         if (STREQL((char *)arg,buf)) {
            sh->dir = sh->devices[i];
            return 0;
         }

      }

      printf("directory not found or does not exist: %s\n",(char *)arg);
      return 1;

   }
   else {

      char buf[SHELL_LINE_LENGTH+8];

      for (int i = 0; i < sh->device_count; ++i) {

         memset(buf,0,sizeof(buf));
         strcpy(buf,"../");
         strcat(buf,sh->devices[i]->name);

         if (STREQL((char *)arg,buf)) {
            sh->dir = sh->devices[i];
            return 0;
         }

      }

   }

   return 3;
}

/* list contents of current device */
int ls(void * arg, shell * sh) {

   if (sh->dir == NULL) {

      for (int i = 0; i < sh->device_count; ++i)
         printf("\033[34m%s \033[0m",sh->devices[i]->name);
      printf("\n");

      return 0;
   }
   else {
      switch (sh->dir->device_type) {

         case SHELL_DEVICE_ADS1015:
            printf("conversion hithresh lothresh config\n");
            break;

         case SHELL_DEVICE_LED1642GW:
            printf("led0  led1  led2  led3  led4  led5  led6\n");
            printf("led7  led8  led9  led10 led11 led12 led13\n");
            printf("led14 led15\n");
            break;

         case SHELL_DEVICE_LM5066:
            printf("mosfet ein vin iin vout temp pin vaux id\n");
            printf("warn_set fault_set gate_mask alert_mask\n");
            printf("config avgs status\n");
            break;

      }

      return 0;
   }

   return 3;
}

/* print contents of given file/register */
int cat(void * arg, shell * sh) {

   if (arg == NULL || *((char *)arg) == '\0') {
      printf("usage: cat [file]\n");
      return 0;
   }

   if (sh->dir == NULL) {
      printf("nothing to cat here :3\n");
      return 2;
   }

   switch (sh->dir->device_type) {

      case SHELL_DEVICE_ADS1015:

         if (STREQL((char *)arg,"conversion")) {
            printf("%u\n",sh->dir->ads->read_conversion());
            return 0;
         }
         else if (STREQL((char *)arg,"hithresh")) {
            printf("%u\n",sh->dir->ads->read_hithresh());
            return 0;
         }
         else if (STREQL((char *)arg,"lothresh")) {
            printf("%u\n",sh->dir->ads->read_lothresh());
            return 0;
         }
         else if (STREQL((char *)arg,"config")) {
            {
               status(arg,sh);
            }
            return 0;
         }
         else {
            printf("no such file\n");
            return 1;
         }

         break;

      case SHELL_DEVICE_LED1642GW:
         printf("library dev left no method of reading status\n");
         return 2;
         break;

      case SHELL_DEVICE_LM5066:

         if (STREQL((char *)arg,"mosfet")) {
            if (sh->dir->lm->get_mosfet())
               printf("on\n");
            else
               printf("off\n");
            return 0;
         }
         else if (STREQL((char *)arg,"ein")) {
            printf("%f\n",sh->dir->lm->get_ein_safe());
         }
         else if (STREQL((char *)arg,"vin")) {
            printf("%f\n",sh->dir->lm->get_vin());
         }
         else if (STREQL((char *)arg,"iin")) {
            printf("%f\n",sh->dir->lm->get_iin());
         }
         else if (STREQL((char *)arg,"vout")) {
            printf("%f\n",sh->dir->lm->get_vout());
         }
         else if (STREQL((char *)arg,"temp")) {
            printf("%f\n",sh->dir->lm->get_temperature());
         }
         else if (STREQL((char *)arg,"pin")) {
            printf("%f\n",sh->dir->lm->get_pin());
         }
         else if (STREQL((char *)arg,"vaux")) {
            printf("%f\n",sh->dir->lm->get_vaux());
         }
         else if (STREQL((char *)arg,"id")) {
            char str[16];
            sh->dir->lm->get_part_string(str,sizeof(str));
            printf("%s\n",str);
         }
         else if (STREQL((char *)arg,"warn_set")) {
            printf("vout uv: %f\n",sh->dir->lm->get_vout_undervoltage_warn_val());
            printf("overtemp: %f\n",sh->dir->lm->get_overtemperature_warn_val());
            printf("vin uv: %f\n",sh->dir->lm->get_undervoltage_warn_val());
            printf("vin ov: %f\n",sh->dir->lm->get_overvoltage_warn_val());
            printf("overcurrent: %f\n",sh->dir->lm->get_overcurrent_warn_val());
            printf("overpower: %f\n",sh->dir->lm->get_overpower_warn_val());
         }
         else if (STREQL((char *)arg,"fault_set")) {
            printf("overtemp: %f\n",sh->dir->lm->get_overtemperature_fault_val());
         }
         else if (STREQL((char *)arg,"gate_mask")) {
         }
         else if (STREQL((char *)arg,"alert_mask")) {
         }
         else if (STREQL((char *)arg,"config")) {
         }
         else if (STREQL((char *)arg,"avgs")) {
            sh->dir->lm->read_values();
            printf("avg vin: %f\n",sh->dir->lm->get_avg_vin());
            printf("avg vout: %f\n",sh->dir->lm->get_avg_vout());
            printf("avg iin: %f\n",sh->dir->lm->get_avg_iin());
            printf("avg pin: %f\n",sh->dir->lm->get_avg_pin());
            return 0;
         }
         else if (STREQL((char *)arg,"status")) {
            status(arg,sh);
            return 0;
         }

         break;

   }

   return 3;

}

/* print back the string given */
int echo(void * arg, shell * sh) {
   if (arg != NULL && *((char *)arg) != '\0')
      printf("%s\n",(char *)arg);
   else
      printf("usage: echo [str]\n");
   return 0;
}

/* print information about board designer */
int credits(void * arg, shell * sh) {
   printf("Bender 2025 Power Board\n");
   printf("Designed and contructed by Chris Dagher\n");
   printf("\n");
   printf("Notable Team Members:\n");

   return 0;
}

/* print the absolute truth */
int megumin(void * arg, shell * sh) {
   printf("is best girl\n");
   return 0;
}

/* return the last result. */
int lr(void * arg, shell * sh) {
   printf("%d\n",sh->lastres);
   return sh->lastres;
}

/* print information on what commands do */
int help(void * arg, shell * sh) {
   printf("General Commands:\n");
   printf("\tcd [dev]           - change device\n");
   printf("\tls                 - list current device contents\n");
   printf("\tcat [file]         - print the contents of a file/register\n");
   printf("\techo [str]         - print the argument back to the user\n");
   printf("\tcredits            - print credits\n");
   printf("\tmegumin            - print the absolute truth\n");
   printf("\tlr                 - print the exit status of the last command\n");
   printf("\terrno              - print the meaning of each exit status\n");
   printf("\tclear              - clear the screen\n");
   printf("\thelp               - print this text\n");
   printf("\tstatus             - print the status of the current device\n");
   printf("\tset [target] [val] - set the value of the provided file/register\n");
   printf("\tget [target]       - alias for cat\n");
   printf("LM5066 Commands:\n");
   printf("ADS1015 Commands:\n");
   printf("LED1642GW Commands:\n");
   printf("\tledon [0-15]       - turn on the given led\n");
   printf("\tledoff [0-15]      - turn off the given led\n");
   return 0;
}

/* print the meaning of each return value */
int errno(void * arg, shell * sh) {
   printf("0   success\n");
   printf("1   not found\n");
   printf("2   operation not permitted\n");
   printf("3   invalid state\n");
   printf("244 incorrect device\n");
   printf("255 command not found\n");
   return 0;
}

/* clear the screen */
int clear(void * arg, shell * sh) {
   printf("\e[2J\e[H\n");
   return 0;
}

int status(void * arg, shell * sh) {

   if (sh->dir == NULL)
      printf("on\n");

   switch (sh->dir->device_type) {

      case SHELL_DEVICE_ADS1015:
         {
            int val = sh->dir->ads->read_config();
            printf("(%x)\n",val);
            printf("conversion: %s\n",val&0x8000?"in progress":"off");
            printf("mux: ");
            switch ((val >> 12)&0x7) {
               case ADS1015_MUX_AIN0_AIN1:
                  printf("AINp = AIN0, AINn = AIN1\n");
                  break;
               case ADS1015_MUX_AIN0_AIN3:
                  printf("AINp = AIN0, AINn = AIN3\n");
                  break;
               case ADS1015_MUX_AIN1_AIN3:
                  printf("AINp = AIN1, AINn = AIN3\n");
                  break;
               case ADS1015_MUX_AIN2_AIN3:
                  printf("AINp = AIN2, AINn = AIN3\n");
                  break;
               case ADS1015_MUX_AIN0_GND:
                  printf("AINp = AIN0, AINn = GND \n");
                  break;
               case ADS1015_MUX_AIN1_GND:
                  printf("AINp = AIN1, AINn = GND \n");
                  break;
               case ADS1015_MUX_AIN2_GND:
                  printf("AINp = AIN2, AINn = GND \n");
                  break;
               case ADS1015_MUX_AIN3_GND:
                  printf("AINp = AIN3, AINn = GND \n");
                  break;
            }
            printf("pga: ");
            switch ((val >> 9)&7) {
               case ADS1015_PGA_6_114:
                  printf("6.114 V\n");
                  break;
               case ADS1015_PGA_4_096:
                  printf("4.096 V\n");
                  break;
               case ADS1015_PGA_2_048:
                  printf("2.048 V\n");
                  break;
               case ADS1015_PGA_1_024:
                  printf("1.024 V\n");
                  break;
               case ADS1015_PGA_0_512:
                  printf("0.512 V\n");
                  break;
               default:
                  printf("0.256 V\n");
                  break;
            }
            printf("mode: %s\n",val&(1<<8)?"single":"continuous");
            printf("data rate: ");
            switch ((val >> 5)&0x7) {
               case ADS1015_DR_128SPS:
                  printf("128 sps\n");
                  break;
               case ADS1015_DR_250SPS:
                  printf("250 sps\n");
                  break;
               case ADS1015_DR_490SPS:
                  printf("490 sps\n");
                  break;
               case ADS1015_DR_920SPS:
                  printf("920 sps\n");
                  break;
               case ADS1015_DR_1600SPS:
                  printf("1600 sps\n");
                  break;
               case ADS1015_DR_2400SPS:
                  printf("2400 sps\n");
                  break;
               default:
                  printf("3300 sps\n");
                  break;
            }
            printf("comparator mode: %s\n",
                  (val&(1<<4))?"window":"traditional");
            printf("comparator polarity: %s\n",
                  (val&(1<<3))?"active high":"active low");
            printf("latching mode comparator: %s\n",
                  (val&0x4)?"on":"off");
            if ((val&0x3) == 3)
               printf("comparator disabled\n");
            else {
               printf("comparator queue: ");
               switch (val&0x3) {
                  case ADS1015_COMP_QUE_ONE:
                     printf("one trigger\n");
                     break;
                  case ADS1015_COMP_QUE_TWO:
                     printf("two triggers\n");
                     break;
                  case ADS1015_COMP_QUE_FOUR:
                     printf("four triggers\n");
                     break;
               }
            }
         }
         break;

      case SHELL_DEVICE_LED1642GW:
         printf("look at it\n");
         return 2;
         break;

      case SHELL_DEVICE_LM5066:

         sh->dir->lm->read_values();
         if (sh->dir->lm->mosfet_shorted()) {
            printf("\033[31m!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            printf("!!!!! MOSFET SHORTED !!!!!\n"
            printf("!!!!!!!!!!!!!!!!!!!!!!!!!!\033[0m\n");
         }
         else
            printf("mosfet: [\033[32mGOOD\033[0m\n");
         printf("temp: %f\n",sh->dir->lm->get_temperature());
         printf("avg vin: %f\n",sh->dir->lm->get_avg_vin());
         printf("avg vout: %f\n",sh->dir->lm->get_avg_vout());
         printf("avg iin: %f\n",sh->dir->lm->get_avg_iin());
         printf("avg pin: %f\n",sh->dir->lm->get_avg_pin());
         if (sh->dir->lm->vout_uv_fault())
            printf("vout uv: [\033[31mFAULT\033[0m]\n");
         else if (sh->dir->lm->vout_uv_warn())
            printf("vout uv: [\033[33mWARN\033[0m]\n");
         else
            printf("vout uv: [\033[32mGOOD\033[0m]\n");
         if (sh->dir->lm->inn_fault())
            printf("iin op: [\033[31mFAULT\033[0m]\n");
         else if (sh->dir->lm->iin_op_warn())
            printf("iin op: [\033[33mWARN\033[0m]\n");
         else
            printf("iin op: [\033[32mGOOD\033[0m]\n");
         if (sh->dir->lm->inn_fault())
            printf("iin op: [\033[31mFAULT\033[0m]\n");
         else if (sh->dir->lm->iin_op_warn())
            printf("iin op: [\033[33mWARN\033[0m]\n");
         else
            printf("iin op: [\033[32mGOOD\033[0m]\n");
         if (sh->dir->lm->vin_uv_fault())
            printf("vin uv: [\033[31mFAULT\033[0m]\n");
         else if (sh->dir->lm->vin_uv_warn())
            printf("vin uv: [\033[33mWARN\033[0m]\n");
         else
            printf("vin uv: [\033[32mGOOD\033[0m]\n");
         if (sh->dir->lm->vin_ov_fault())
            printf("vin ov: [\033[31mFAULT\033[0m]\n");
         else if (sh->dir->lm->vin_ov_warn())
            printf("vin ov: [\033[33mWARN\033[0m]\n");
         else
            printf("vin ov: [\033[32mGOOD\033[0m]\n");
         if (sh->dir->lm->power_good())
            printf("power good: [TRUE]\n");
         else 
            printf("power good: [FALSE]\n");
         if (sh->dir->lm->temp_fault())
            printf("overtemp: [\033[31mFAULT\033[0m]\n");
         else if (sh->dir->lm->ot_warn())
            printf("overtemp: [\033[33mWARN\033[0m]\n");
         else
            printf("overtemp: [\033[32mGOOD\033[0m]\n");
         if (sh->dir->lm->timer_latched())
            printf("timer latched: [TRUE]\n");
         else 
            printf("timer latched: [FALSE]\n");
         if (sh->dir->lm->config_preset())
            printf("config preset: [TRUE]\n");
         else 
            printf("config preset: [FALSE]\n");
         if (sh->dir->lm->device_off())
            printf("device off: [TRUE]\n");
         else 
            printf("device off: [FALSE]\n");
         if (sh->dir->lm->cml_fault())
            printf("cml: [\033[31mFAULT\0330m]\n");
         else 
            printf("cml: [\033[32mGOOD\0330m]\n");
         if (sh->dir->lm->cb_fault())
            printf("cb: [\033[31mFAULT\0330m]\n");
         else 
            printf("cb: [\033[32mGOOD\0330m]\n");

         break;
   }

   return 0;
}

int set(void * arg, shell * sh) {

   if (arg == NULL || *((char *)((shell_pair *)arg)->arg0) == '\0') {
      printf("usage: set [target] [value]\n");
      return 0;
   }

   if (sh->dir == NULL) {
      printf("no targets\n");
      return 2;
   }

   switch (sh->dir->device_type) {

      case SHELL_DEVICE_ADS1015:

         if (STREQL("conversion",(char *)((shell_pair *)arg)->arg0)) {
            printf("cannot write to conversion register\n");
            return 2;
         }
         else if (STREQL("hithresh",(char *)((shell_pair *)arg)->arg0)) {
            sh->dir->ads->write_hithresh((long)((shell_pair *)arg)->arg1);
            return 0;
         }
         else if (STREQL("lothresh",(char *)((shell_pair *)arg)->arg0)) {
            sh->dir->ads->write_lothresh((long)((shell_pair *)arg)->arg1);
            return 0;
         }
         else if (STREQL("config",(char *)((shell_pair *)arg)->arg0)) {
            sh->dir->ads->write_config((long)((shell_pair *)arg)->arg1);
            return 0;
         }
         else {
            printf("target not found\n");
            return 1;
         }

         break;

      case SHELL_DEVICE_LED1642GW:

         if (STREQL("led0",(char *)((shell_pair *)arg)->arg0)) {
            sh->dir->ads->setLedOn(0,(long)((shell_pair *)arg)->arg1);
            return 0;
         }
         else if (STREQL("led1",(char *)((shell_pair *)arg)->arg0)) {
            sh->dir->ads->setLedOn(1,(long)((shell_pair *)arg)->arg1);
            return 0;
         }
         else if (STREQL("led2",(char *)((shell_pair *)arg)->arg0)) {
            sh->dir->ads->setLedOn(2,(long)((shell_pair *)arg)->arg1);
            return 0;
         }
         else if (STREQL("led3",(char *)((shell_pair *)arg)->arg0)) {
            sh->dir->ads->setLedOn(3,(long)((shell_pair *)arg)->arg1);
            return 0;
         }
         else if (STREQL("led4",(char *)((shell_pair *)arg)->arg0)) {
            sh->dir->ads->setLedOn(4,(long)((shell_pair *)arg)->arg1);
            return 0;
         }
         else if (STREQL("led5",(char *)((shell_pair *)arg)->arg0)) {
            sh->dir->ads->setLedOn(5,(long)((shell_pair *)arg)->arg1);
            return 0;
         }
         else if (STREQL("led6",(char *)((shell_pair *)arg)->arg0)) {
            sh->dir->ads->setLedOn(6,(long)((shell_pair *)arg)->arg1);
            return 0;
         }
         else if (STREQL("led7",(char *)((shell_pair *)arg)->arg0)) {
            sh->dir->ads->setLedOn(7,(long)((shell_pair *)arg)->arg1);
            return 0;
         }
         else if (STREQL("led8",(char *)((shell_pair *)arg)->arg0)) {
            sh->dir->ads->setLedOn(8,(long)((shell_pair *)arg)->arg1);
            return 0;
         }
         else if (STREQL("led9",(char *)((shell_pair *)arg)->arg0)) {
            sh->dir->ads->setLedOn(9,(long)((shell_pair *)arg)->arg1);
            return 0;
         }
         else if (STREQL("led10",(char *)((shell_pair *)arg)->arg0)) {
            sh->dir->ads->setLedOn(10,(long)((shell_pair *)arg)->arg1);
            return 0;
         }
         else if (STREQL("led11",(char *)((shell_pair *)arg)->arg0)) {
            sh->dir->ads->setLedOn(11,(long)((shell_pair *)arg)->arg1);
            return 0;
         }
         else if (STREQL("led12",(char *)((shell_pair *)arg)->arg0)) {
            sh->dir->ads->setLedOn(12,(long)((shell_pair *)arg)->arg1);
            return 0;
         }
         else if (STREQL("led13",(char *)((shell_pair *)arg)->arg0)) {
            sh->dir->ads->setLedOn(13,(long)((shell_pair *)arg)->arg1);
            return 0;
         }
         else if (STREQL("led14",(char *)((shell_pair *)arg)->arg0)) {
            sh->dir->ads->setLedOn(14,(long)((shell_pair *)arg)->arg1);
            return 0;
         }
         else if (STREQL("led15",(char *)((shell_pair *)arg)->arg0)) {
            sh->dir->ads->setLedOn(15,(long)((shell_pair *)arg)->arg1);
            return 0;
         }
         else {
            printf("target not found\n");
            return 1;
         }

         break;

      case SHELL_DEVICE_LM5066:
         break;

   }

   return 3;
}

int get(void * arg, shell * sh) {

   if (arg == NULL || *((char *)arg) == '\0') {
      printf("usage: get [target]\n");
      return 0;
   }

   if (sh->dir == NULL) {
      printf("no file in root\n");
      return 2;
   }

   cat(arg,sh);
   return 0;
}

int ledon(void * arg, shell * sh) {

   if ((long)arg > 15 || (long)arg < 0) {
      printf("invalid led selected\n");
      return 2;
   }

   sh->dir->led->setLedOn((long)arg);

   return 0;
}

int ledoff(void * arg, shell * sh) {

   if ((long)arg > 15 || (long)arg < 0) {
      printf("invalid led selected\n");
      return 2;
   }

   sh->dir->led->setLedOff((long)arg);

   return 0;
}

