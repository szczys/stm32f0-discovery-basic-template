#ifndef CLI_H_
#define CLI_H_

#define countof(a)   (sizeof(a) / sizeof(*(a)))

/**
 * Definition of a single element
 */
typedef struct
{
    /**
     * Name of the element
     */
    const char     *name;

    /**
     * Value of the elment
     */
    uint8_t        value;

} element_t;

/**
 * All elements of the list
 */
typedef struct
{
    /**
     * Number of elements
     */
    unsigned char      count;

    /**
     * The elements
     */
    element_t          elements[];
} list_t;

extern char *welcome_msg;
extern list_t flags;
extern list_t options;
extern list_t inputs;
extern list_t outputs;

/* "shell_process" takes a command line as input,
    and "libshell" does the rest for you (check if
    the command is defined, call the callback function,
    provide arguments form the commandline to the callback)
*/
int shell_process(char *cmd_line);

int serve_command_promt(char *buffer, int bufferLength, const char *prompt);

#endif /* CLI_H_ */
