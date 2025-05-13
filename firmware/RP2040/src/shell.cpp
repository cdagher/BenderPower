
#include "shell.h"

int lmod(int k, int n) {

   k %= n;

   return (k < 0 ? k + n : k);

}

void circular_buffer_add(char * val, struct circular_buffer * buf) {

   strcpy(buf->buf[buf->pos],val);

   buf->pos = lmod((buf->pos - 1),SHELL_SCROLLBACK);

}

void circular_buffer_get(char * val, uint8_t pos, struct circular_buffer * buf) {

   strcpy(val,buf->buf[lmod((buf->pos + pos),SHELL_SCROLLBACK)]);

}

shell::shell() {

   this->scrollback = 0;
   this->linepos = 0;
   this->linemax = 0;
   this->device_count = 0;
   this->dir = NULL;
   this->commands = NULL;
   this->lastres = 0;
   this->typed = 0;

   memset(buf,0,sizeof(buf));
}

void shell::delete_character() {
   if (linepos == 0)
      return;

   memmove(buf+linepos-1, buf+linepos,1 + (linemax - linepos));

   linepos -= 1;
   linemax -= 1;
}

void shell::add_device(struct shell_device * device) {

   if (device_count == SHELL_DEVICE_COUNT)
      return;

   devices[device_count] = device;
   device_count += 1;
}

void shell::add_command(struct shell_command * command) {


   command->next = commands;
   commands = command;

}

struct shell_command * shell::print_partial_matches() {

   struct shell_command * collected_matches = NULL;

   struct shell_command * curr = commands, * next, * prev = NULL;

   while (curr) {

      if (0 == strncmp(buf,curr->name,strlen(buf))) {

         next = curr->next;
         curr->next = collected_matches;
         collected_matches = curr;

         if (prev)
            prev->next = next;

         if (curr == commands)
            commands = next;

         curr = next;

      }
      else {
         prev = curr;
         curr = curr->next;
      }

   }

   if (collected_matches == NULL)
      return NULL;

   if (collected_matches->next == NULL) {
      collected_matches->next = commands;
      commands = collected_matches;

      return collected_matches;
   }

   curr = collected_matches;
   printf("\n");
   while (curr->next) {

      printf("%s\t",curr->name);

      curr = curr->next;
   }

   printf("%s\n",curr->name);
   curr->next = commands;
   commands = collected_matches;

   return NULL;
}

int shell::execute_command() {

   struct shell_command * curr = commands;

   while (curr) {

      int cspn = strcspn(buf," ");

      if (strncmp(curr->name,buf,cspn) == 0 &&
          strlen(curr->name) <= cspn) {

         void * arg;
         struct shell_pair pair;

         if (!(curr->device_type == SHELL_DEVICE_ANY ||
             (dir && curr->device_type == dir->device_type))) {
            printf("command called from incorrect device\n");
            return -2;
         }

         char * arg_start = buf + cspn;
         while (*arg_start == ' ') arg_start += 1;

         switch (curr->input_type) {
            case SHELL_INPUT_NUMBER:
               arg = (void *)strtol(arg_start,NULL,0);
               break;
            case SHELL_INPUT_STRING:
               arg_start[strcspn(arg_start," ")] = '\0';
               arg = (void *)arg_start;
            break;
            case SHELL_INPUT_NUMBER2:
               pair.arg0 = (void *)strtol(arg_start,&arg_start,0);
               pair.arg1 = (void *)strtol(arg_start,NULL,0);
               arg = (void *)&pair;
            break;
            case SHELL_INPUT_STRING2:
               arg_start[strcspn(arg_start," ")] = '\0';
               pair.arg0 = (void *)arg_start;
               arg_start += strlen(arg_start) + 1;
               arg_start[strcspn(arg_start," ")] = '\0';
               pair.arg1 = (void *)arg_start;
               arg = (void *)&pair;
            break;
            case SHELL_INPUT_NUM_STR:
               pair.arg0 = (void *)strtol(arg_start,&arg_start,0);
               arg_start[strcspn(arg_start," ")] = '\0';
               pair.arg1 = (void *)arg_start;
               arg = (void *)&pair;
            break;
            case SHELL_INPUT_STR_NUM:
               arg_start[strcspn(arg_start," ")] = '\0';
               pair.arg0 = (void *)arg_start;
               arg_start += strlen(arg_start) + 1;
               pair.arg1 = (void *)strtol(arg_start,NULL,0);
               arg = (void *)&pair;
            break;
            case SHELL_INPUT_NONE:
            default:
               arg = NULL;
               break;
         }

         return curr->command(arg,this);

      }

      curr = curr->next;
   }

   printf("command not found\n");
   return -1;
}

void shell::poll_input() {

   int c = 0;
   int prev[2] = { 0, 0 };

   while (( c = SHELL_GETCHAR() ) != SHELL_INPUT_DONE) {

      switch (c) {

         /* special check for up arrow key pressed */
         case 'A':
            if (prev[1] == SHELL_UP_KEY[0] &&
                prev[0] == SHELL_UP_KEY[1]) {

               memset(buf,0,SHELL_LINE_LENGTH);

               circular_buffer_get(buf,++scrollback,&scrollback_buf);

               linepos = strlen(buf);
               linemax = linepos;

            }
            else
               goto default_jmp;
         break;

         /* special check for down arrow key pressed */
         case 'B':
            if (prev[1] == SHELL_UP_KEY[0] &&
                prev[0] == SHELL_UP_KEY[1]) {

               memset(buf,0,SHELL_LINE_LENGTH);

               circular_buffer_get(buf,scrollback--,&scrollback_buf);

               linepos = strlen(buf);
               linemax = linepos;
            }
            else
               goto default_jmp;
         break;

         /* special check for right arrow key pressed */
         case 'C':
            if (prev[1] == SHELL_UP_KEY[0] &&
                prev[0] == SHELL_UP_KEY[1]) {

               if (linemax != SHELL_LINE_LENGTH-1 || buf[linepos-1] == '[')
                  delete_character();

               if (linepos == linemax)
                  break;

               linepos += 1;
            }
            else
               goto default_jmp;
         break;

         /* special check for left arrow key pressed */
         case 'D':
            if (prev[1] == SHELL_UP_KEY[0] &&
                prev[0] == SHELL_UP_KEY[1]) {

               if (linemax != SHELL_LINE_LENGTH-1 || buf[linepos-1] == '[')
                  delete_character();

               if (linepos == 0)
                  break;

               linepos -= 1;
            }
            else
               goto default_jmp;
         break;

         case SHELL_BACKSPACE_0:
         case SHELL_BACKSPACE_1:
         {

            delete_character();

         }
         break;

         case SHELL_TAB:
         {
            struct shell_command * res = print_partial_matches();

            if (res != NULL) {
               int reslen = strlen(res->name);
               memcpy(buf, res->name, reslen);
               buf[reslen] = ' ';
               linepos = reslen + 1;
               linemax = reslen + 1;
            }

         }
         break;

         case SHELL_ENTER:
         {

            printf("\n");

            if (buf[0] == '\0')
               break;

            lastres = execute_command();

            if (typed)
               circular_buffer_add(buf,&scrollback_buf);

            memset(buf,0,SHELL_LINE_LENGTH);
            linepos = 0;
            linemax = 0;
            scrollback = 0;
            typed = 0;

         }
         break;

         default:
default_jmp:
         {

            if (linemax == SHELL_LINE_LENGTH-1) {
               printf("\a");
               break;
            }

            if (c < 32)
               break;

            memmove(buf+linepos+1,buf+linepos,(linemax - linepos));

            buf[linepos] = c;

            linepos += 1;
            linemax += 1;

            if (c != '[')
               typed = 1;

         }
         break;

      }

      prev[1] = prev[0];
      prev[0] = c;

      buf[linemax] = '\0';

      printf("\e[2K\r%s> %s",(dir ? dir->name : ""),buf);

      for (int i = linepos; i != linemax; ++i)
         printf("\e[1D");

      stdio_flush();


   }

}


