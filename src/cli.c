#include "cli.h"
#include "conio.h"

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

int shell_process(char *cmd_line)
{
    int ret = shell_process_cmds(&microcli_shell_cmds, cmd_line);
    
    switch(ret)
    {
       case SHELL_PROCESS_ERR_CMD_UNKN:
          // Unknown command
          cio_printf("ERROR: Unknown command\n\r");
          break;
       case SHELL_PROCESS_ERR_ARGS_LEN:
          // Argument to long
          cio_printf("ERROR: Argument to long\n\r");
          break;
       case SHELL_PROCESS_ERR_ARGS_MAX:
          // Too many arguments
          cio_printf("ERROR: Too many arguments\n\r");
          break;
       default:
          // OK
          break;
    }
    
    return ret;
}

int shell_cmd_help(shell_cmd_args *args)
{
    // OPTIONAL: perform check on given arguments ...

    int i;

    for(i = 0; i < microcli_shell_cmds.count; i++) {
        cio_printf("%s, %s\n\r", microcli_shell_cmds.cmds[i].cmd, microcli_shell_cmds.cmds[i].desc);
    }

    cio_print((char *)"OK\n\r");

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
