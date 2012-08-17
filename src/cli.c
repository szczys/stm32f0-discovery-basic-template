#include "cli.h"
#include "conf.h"
#include "shell.h"
#include "conio.h"
#include "usart.h"
#include "stdbool.h"
#include <string.h>
#include "wifi.h"

char* welcome_msg = 
      "\r\n\r\nMicroCLI 0.1.0 (" __DATE__ ")\r\n"
      "Type \"copyright\", \"credits\" or \"license\" for more information.\r\n"
      "MicroCLI -- An embedded command line interpreter.\r\n"
      "about            -> Introduction and overview of MicroCLI's features.\r\n"
      "list commands    -> Get a list of the builtin commands.\r\n"
      "help commandxy   -> Details about 'commandxy'.\r\n";

int shell_cmd_list(shell_cmd_args *args);
int shell_cmd_argt(shell_cmd_args *args);

#ifdef WIFI_CONNECTED
int wifi_cmd_connect(shell_cmd_args *args);
int wifi_cmd_disconnect(shell_cmd_args *args);
#define WIFI_COMMANDS 2
#else
#define WIFI_COMMANDS 0
#endif

shell_cmds microcli_shell_cmds = {
     .count 	= 2 + WIFI_COMMANDS,
     .cmds	= {
          {
               .cmd		= "list",
               .desc	= "list available inputs/outputs/commands/options/flags",
               .func 	= shell_cmd_list,
          },
          {
               .cmd		= "argt",
               .desc	= "print back given arguments",
               .func 	= shell_cmd_argt,
          },
#ifdef WIFI_CONNECTED        
          {
              .cmd		= "CONNECT",
              .desc	    = "",
              .func 	= wifi_cmd_connect,
          },
          {
             .cmd		= "DISCONNECT",
             .desc	    = "",
             .func 	    = wifi_cmd_disconnect,
          },
#endif
     },
};

int serve_command_promt(char *buffer, int bufferLength, const char *prompt)
{
  static char initialized = false;
  static char *p;
  
  if(!initialized)
  {
      p = buffer;
      *p = '\0';
      cio_printf("%s", prompt);
      initialized = true;
  }

  while (!serial_rb_empty(&rxbuf))
  {
      char c = serial_rb_read(&rxbuf);
      switch (c)
      {
        // https://en.wikipedia.org/wiki/ASCII#ASCII_control_characters
        case '\n' :
        case '\r' :
          cio_printf("\r\n");
          initialized = false;
          return shell_str_len(buffer);

        case '\b' :
          if (p > buffer)
            *--p = '\0';
          cio_printf("\b \b");
          break;

        case 0x15 : // CTRL-U
          while (p != buffer)
          {
            cio_printf("\b \b");
            --p;
          }
          *p = '\0';
          break;

        case '\e': // ESC
          cio_printf("^[\r\n");
          initialized = false;
          break;

        default : 
          if (p < buffer + bufferLength - 1 && c >= ' ' && c < 127)
          { 
            *p++ = c;
            *p = '\0';
            cio_printf("%c", c); 
          }
          else
            cio_printf("%c", c); 
          break;
      }
  }

  return 0;
}

int shell_process(char *cmd_line)
{
    int ret = shell_process_cmds(&microcli_shell_cmds, cmd_line);
    
    switch(ret)
    {
       case SHELL_PROCESS_ERR_CMD_UNKN:
          // Unknown command
          cio_printf("ERROR: Unknown command: '%s'\r\n", cmd_line);
          break;
       case SHELL_PROCESS_ERR_ARGS_LEN:
          // Argument to long
          cio_printf("ERROR: Argument to long: '%s'\r\n", cmd_line);
          break;
       case SHELL_PROCESS_ERR_ARGS_MAX:
          // Too many arguments
          cio_printf("ERROR: Too many arguments: '%s'\r\n", cmd_line);
          break;
       default:
          // OK
          break;
    }
    return ret;
}

int shell_cmd_list(shell_cmd_args *args)
{
    if(args->count == 1)
    {
        if(strcmp(args->args[0].val, "commands") >= 0)
        {
            for(int i = 0; i < microcli_shell_cmds.count-WIFI_COMMANDS; i++) { // -2 -> don't list CONNECT, DISCONNECT
                cio_printf("%s --> %s\r\n", microcli_shell_cmds.cmds[i].cmd, microcli_shell_cmds.cmds[i].desc);
            }
        }
    }

    return 0;
}

int shell_cmd_argt(shell_cmd_args *args)
{
    // OPTIONAL: perform check on given arguments ...

	int i;

    cio_printf((char *)"OK\r\nargs given:\r\n");

    for(i = 0; i < args->count; i++) {
         cio_printf(" - %s\r\n", args->args[i].val);
    }

    return 0;
}

#ifdef WIFI_CONNECTED
int wifi_cmd_connect(shell_cmd_args *args)
{
    if(args->count == 4)
    {
        // CONNECT 0 1 192.168.1.2 45552
        // if(shell_str_cmp(line_buffer,"CONNECT", len, 7) == 1)
        cid = args->args[1].val[0]; //TODO: check for valid connection 0..9 A..F
        //cio_printf("i: %i char: %c\r\n", cid);
  
        cio_printf("\x1bS%c%s\x1b""E", cid, welcome_msg);
        cio_printf("\x1bS%c%s\x1b""E", cid, "microcli> \r\n");
    }
    else
    {
        cio_printf("ERROR: CONNECT message has wrong format\r\n");
    }
    return 0;
}

int wifi_cmd_disconnect(shell_cmd_args *args)
{
    // DISCONNECT 1..F
    if(args->count == 1)
    {
        // set to disconnect in connection list
        // args->args[0].val[0];
    }
    else
    {
        cio_printf("ERROR: DISCONNECT message has wrong format\r\n");
    }
    return 0;
}
#endif
