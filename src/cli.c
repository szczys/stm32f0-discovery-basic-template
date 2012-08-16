#include "cli.h"
#include "conio.h"
#include "usart.h"
#include "stdbool.h"

int shell_cmd_help(shell_cmd_args *args);
int shell_cmd_argt(shell_cmd_args *args);

shell_cmds microcli_shell_cmds = {
     .count 	= 2,
     .cmds	= {
          {
               .cmd		= "h",
               .desc	= "list available commands",
               .func 	= shell_cmd_help,
          },
          {
               .cmd		= "argt",
               .desc	= "print back given arguments",
               .func 	= shell_cmd_argt,
          },
     },
};

int serve_command_promt(char *buffer, int bufferLength, const char *prompt)
{
  static char active_prompt = false;
  static char *p;
  
  if(!active_prompt)
  {
      p = buffer;
      *p = '\0';
      cio_printf("%s", prompt);
      active_prompt = true;
  }

  while (!serial_rb_empty(&rxbuf))
  {
      char c = serial_rb_read(&rxbuf);
      switch (c)
      {
        // https://en.wikipedia.org/wiki/ASCII#ASCII_control_characters
        case '\n' :
        case '\r' :
          cio_printf("\n\r");
          active_prompt = false;
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
          cio_printf("^[\n\r");
          active_prompt = false;
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
          cio_printf("ERROR: Unknown command: '%s'\n\r", cmd_line);
          break;
       case SHELL_PROCESS_ERR_ARGS_LEN:
          // Argument to long
          cio_printf("ERROR: Argument to long: '%s'\n\r", cmd_line);
          break;
       case SHELL_PROCESS_ERR_ARGS_MAX:
          // Too many arguments
          cio_printf("ERROR: Too many arguments: '%s'\n\r", cmd_line);
          break;
       default:
          // OK
          break;
    }
    cio_printf("[end]\n\r");
    return ret;
}

int shell_cmd_help(shell_cmd_args *args)
{
    // OPTIONAL: perform check on given arguments ...

    int i;

    for(i = 0; i < microcli_shell_cmds.count; i++) {
        cio_printf("%s, %s\n\r", microcli_shell_cmds.cmds[i].cmd, microcli_shell_cmds.cmds[i].desc);
    }

    return 0;
}

int shell_cmd_argt(shell_cmd_args *args)
{
    // OPTIONAL: perform check on given arguments ...

	int i;

    cio_print((char *)"OK\n\rargs given:\n\r");

    for(i = 0; i < args->count; i++) {
         cio_printf(" - %s\n\r", args->args[i].val);
    }

    return 0;
}
