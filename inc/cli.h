#ifndef CLI_H_
#define CLI_H_

extern char* welcome_msg;

/* "shell_process" takes a command line as input,
    and "libshell" does the rest for you (check if
    the command is defined, call the callback function,
    provide arguments form the commandline to the callback)
*/
int shell_process(char *cmd_line);

int serve_command_promt(char *buffer, int bufferLength, const char *prompt);

#endif /* CLI_H_ */
