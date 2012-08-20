/*
 * This file is part of MicroCLI.
 *
 * Copyright (C) 2012 Christian Jann <christian.jann@ymail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "cli.h"
#include "conf.h"
#include "shell.h"
#include "conio.h"
#include "usart.h"
#include "stdbool.h"
#include <string.h>
#include "wifi.h"
#include "main.h"

char* welcome_msg =
    "\r\n\r\nMicroCLI 0.1.0 (" __DATE__ ")\r\n"
    "Type \"copyright\", \"credits\" or \"license\" for more information.\r\n"
    "MicroCLI -- An embedded command line interpreter.\r\n"
    "about            -> Introduction and overview of MicroCLI's features.\r\n"
    "list commands    -> Get a list of the builtin commands.\r\n"
    "help commandxy   -> Details about 'commandxy'.\r\n";

int shell_cmd_list(shell_cmd_args *args);
int shell_cmd_argt(shell_cmd_args *args);
int shell_cmd_deviceinfo(shell_cmd_args *args);
int shell_cmd_ping(shell_cmd_args *args);
int shell_cmd_get(shell_cmd_args *args);
int shell_cmd_enable(shell_cmd_args *args);
int shell_cmd_disable(shell_cmd_args *args);
int shell_cmd_toggle(shell_cmd_args *args);
int shell_cmd_uptime(shell_cmd_args *args);

#ifdef WIFI_CONNECTED
int wifi_cmd_connect(shell_cmd_args *args);
int wifi_cmd_disconnect(shell_cmd_args *args);
int wifi_cmd_error(shell_cmd_args *args);
#define WIFI_COMMANDS 3
#else
#define WIFI_COMMANDS 0
#endif

shell_cmds microcli_shell_cmds =
{
    .count = 8 + WIFI_COMMANDS,
    .cmds  = {
        {
            .cmd     = "list",
            .desc    = "List available inputs/outputs/commands/options/flags/connections",
            .func    = shell_cmd_list,
        },
        {
            .cmd     = "deviceinfo",
            .desc    = "Show device information",
            .func    = shell_cmd_deviceinfo,
        },
        {
            .cmd     = "enable",
            .desc    = "Enable an output or an option",
            .func    = shell_cmd_enable,
        },
        {
            .cmd     = "disable",
            .desc    = "Disable an output or an option",
            .func    = shell_cmd_disable,
        },
        {
            .cmd     = "toggle",
            .desc    = "Toggle an output (LEDx)",
            .func    = shell_cmd_toggle,
        },
        {
            .cmd     = "get",
            .desc    = "Get LED0 or GET BUTTON1",
            .func    = shell_cmd_get,
        },
        {
            .cmd     = "ping",
            .desc    = "Send a 'pong' back",
            .func    = shell_cmd_ping,
        },
        {
            .cmd     = "uptime",
            .desc    = "Tell how long the system has been running.",
            .func    = shell_cmd_uptime,
        },
#ifdef WIFI_CONNECTED
        {
            .cmd     = "CONNECT",
            .desc    = "",
            .func    = wifi_cmd_connect,
        },
        {
            .cmd     = "DISCONNECT",
            .desc    = "",
            .func    = wifi_cmd_disconnect,
        },
        {
            .cmd     = "ERROR:",
            .desc    = "",
            .func    = wifi_cmd_error,
        },
#endif
    },
};

list_t outputs =
{
    .count    = 6,
    .elements = {
        {
            .name     = "LED3",
            .value    = LED3,
        },
        {
            .name     = "LED4",
            .value    = LED4,
        },
        {
            .name     = "RELAIS1",
            .value    = RELAIS1,
        },
        {
            .name     = "RELAIS2",
            .value    = RELAIS2,
        },
        {
            .name     = "OUT_12V_1",
            .value    = OUT_12V_1,
        },
        {
            .name     = "OUT_12V_2",
            .value    = OUT_12V_2,
        },
    },
};

list_t inputs =
{
    .count    = 3,
    .elements = {
        {
            .name     = "BUTTON1",
            .value    = BUTTON_USER,
        },
        {
            .name     = "IN_12V_1",
            .value    = IN_12V_1,
        },
        {
            .name     = "IN_12V_2",
            .value    = IN_12V_2,
        },
    },
};

list_t flags =
{
    .count    = 4,
    .elements = {
        {
            .name     = "NO_FREE_MEMORY",
            .value    = 0,
        },
        {
            .name     = "LOW_BATTERY",
            .value    = 0,
        },
        {
            .name     = "WIFI_MOD_ERR",
            .value    = 0,
        },
        {
            .name     = "I'M_BORED",
            .value    = 1,
        },
    },
};

list_t options =
{
    .count    = 2,
    .elements = {
        {
            .name     = "TOGGLE_LEDS",
            .value    = 1,
        },
        {
            .name     = "REALTIME_MSG",
            .value    = 1,
        },
    },
};

int serve_command_promt(char *buffer, int bufferLength, const char *prompt)
{
    static char initialized = false;
    static char *p;

    if (!initialized)
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
        case '\n':
        case '\r':
            cio_printf("\r\n");
            initialized = false;
            return shell_str_len(buffer);

        case '\b':
            if (p > buffer)
                *--p = '\0';
            cio_printf("\b \b");
            break;

        case 0x15: // CTRL-U
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

        default:
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

    switch (ret)
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
    if (args->count == 1)
    {
        if (strcmp(args->args[0].val, "commands") == 0)
        {
            for (int i = 0; i < microcli_shell_cmds.count - WIFI_COMMANDS; i++) // -2 -> don't list CONNECT, DISCONNECT
            {
                cio_printf("%s --> %s\r\n", microcli_shell_cmds.cmds[i].cmd, microcli_shell_cmds.cmds[i].desc);
            }
        }
        else if (strcmp(args->args[0].val, "inputs") == 0)
        {
            for (int i = 0; i < inputs.count; i++)
            {
                cio_printf("%s\r\n", inputs.elements[i].name);
            }

        }
        else if (strcmp(args->args[0].val, "outputs") == 0)
        {
            for (int i = 0; i < outputs.count; i++)
            {
                cio_printf("%s\r\n", outputs.elements[i].name);
            }

        }
        else if (strcmp(args->args[0].val, "options") == 0)
        {
            for (int i = 0; i < options.count; i++)
            {
                cio_printf("%s\r\n", options.elements[i].name);
            }

        }
        else if (strcmp(args->args[0].val, "flags") == 0)
        {
            for (int i = 0; i < flags.count; i++)
            {
                cio_printf("%s\r\n", flags.elements[i].name);
            }

        }
#ifdef WIFI_CONNECTED
        else if (strcmp(args->args[0].val, "connections") == 0)
        {
            cio_print("CID IP:PORT\r\n");
            for (int i = 0; i < 16; i++)
            {
                cio_printf("%c   %s:%s\r\n", wifi_connections[i].cid, wifi_connections[i].ip, wifi_connections[i].port);
            }
        }
#endif
        else
        {
            cio_printf("[1] Unknown argument: '%s'\r\n", args->args[0].val);
        }
    }

    return 0;
}

int shell_cmd_deviceinfo(shell_cmd_args *args)
{
    cio_print("Device Type: F0-Discovery Demo\r\n"
              "Hardware Revision: 1.0\r\n"
              "Firmware Version: 1.8\r\n"
              "Build date: " __DATE__ "\r\n"
              "URL: www.jann.cc\r\n");
    return 0;
}

int shell_cmd_ping(shell_cmd_args *args)
{
    cio_print("pong\r\n");
    return 0;
}

int shell_cmd_get(shell_cmd_args *args)
{
    for (int i = 0; i < inputs.count; i++)
    {
        if (strcmp(args->args[0].val, inputs.elements[i].name) == 0)
        {
            cio_printf("%i\r\n", STM_EVAL_PBGetState(inputs.elements[i].value));
            return 0;
        }
    }
    for (int i = 0; i < flags.count; i++)
    {
        if (strcmp(args->args[0].val, flags.elements[i].name) == 0)
        {
            cio_printf("%i\r\n", flags.elements[i].value);
            return 0;
        }
    }
    for (int i = 0; i < options.count; i++)
    {
        if (strcmp(args->args[0].val, options.elements[i].name) == 0)
        {
            cio_printf("%i\r\n", options.elements[i].value);
            return 0;
        }
    }
    for (int i = 0; i < outputs.count; i++)
    {
        if (strcmp(args->args[0].val, outputs.elements[i].name) == 0)
        {
            cio_printf("%i\r\n", GPIO_ReadOutputDataBit(GPIO_PORT[outputs.elements[i].value], GPIO_PIN[outputs.elements[i].value]));
            return 0;
        }
    }
    return 0;
}

int shell_cmd_enable(shell_cmd_args *args)
{
    for (int i = 0; i < options.count; i++)
    {
        if (strcmp(args->args[0].val, options.elements[i].name) == 0)
        {
            options.elements[i].value = 1;
            return 0;
        }
    }
    for (int i = 0; i < outputs.count; i++)
    {
        if (strcmp(args->args[0].val, outputs.elements[i].name) == 0)
        {
            STM_EVAL_LEDOn(outputs.elements[i].value);
            return 0;
        }
    }
    return 0;
}

int shell_cmd_disable(shell_cmd_args *args)
{
    for (int i = 0; i < options.count; i++)
    {
        if (strcmp(args->args[0].val, options.elements[i].name) == 0)
        {
            options.elements[i].value = 0;
            return 0;
        }
    }
    for (int i = 0; i < outputs.count; i++)
    {
        if (strcmp(args->args[0].val, outputs.elements[i].name) == 0)
        {
            STM_EVAL_LEDOff(outputs.elements[i].value);
            return 0;
        }
    }
    return 0;
}

int shell_cmd_toggle(shell_cmd_args *args)
{
    for (int i = 0; i < outputs.count; i++)
    {
        if (strcmp(args->args[0].val, outputs.elements[i].name) == 0)
        {
            STM_EVAL_LEDToggle(outputs.elements[i].value);
            return 0;
        }
    }
    return 0;
}

int shell_cmd_uptime(shell_cmd_args *args)
{
    int seconds = ((int)(uptime)) % 60;
    int minutes = ((int)(uptime / (60))) % 60;
    int hours   = ((int)(uptime / (60 * 60))) % 24;
    int days    = ((int)(uptime / (60 * 60 * 24))) % 7;
    //int weeks   = ((int)(uptime / (60*60*24*7))) % 52;
    int years   = ((int)(uptime / (60 * 60 * 24 * 365))) % 10;
    //int decades = ((int)(uptime / (60*60*24*365*10)));
    cio_printf("System Up Time: %i Years, %i Days, %i Hours, %i Minutes, %i Seconds\r\n",
               years, days, hours, minutes, seconds);
    return 0;
}

#ifdef WIFI_CONNECTED
int wifi_cmd_connect(shell_cmd_args *args)
{
    if (args->count == 4)
    {
        // CONNECT 0 1 192.168.1.2 45552
        // if(shell_str_cmp(line_buffer,"CONNECT", len, 7) == 1)
        cid = args->args[1].val[0]; //TODO: check for valid connection 0..9 A..F
        //cio_printf("i: %i char: %c\r\n", cid);

        cio_printf("\x1bS%c%s\x1b""E", cid, welcome_msg);
        cio_printf("\x1bS%c%s\x1b""E", cid, "microcli> \r\n");

        for (int i = 0; i < 16; i++)
        {
            if (wifi_connections[i].cid == cid)
            {
                strncpy(wifi_connections[i].ip, args->args[2].val, 16);
                strncpy(wifi_connections[i].port, args->args[3].val, 10);
                wifi_connections[i].state = CONNECTED;
                break;
            }
        }
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
    if (args->count == 1)
    {
        for (int i = 0; i < 16; i++)
        {
            if (wifi_connections[i].cid == args->args[0].val[0])
            {
                strcpy(wifi_connections[i].ip, "0.0.0.0");
                strcpy(wifi_connections[i].port, "0000");
                wifi_connections[i].state = DISCONNECTED;
                break;
            }
        }
    }
    else
    {
        cio_printf("ERROR: DISCONNECT message has wrong format\r\n");
    }
    return 0;
}

int wifi_cmd_error(shell_cmd_args *args)
{
    //ERROR: Unknown command: 'ERROR: Unknown cand: ' 'On command: 'E'''T'
    //ERROR: INVALID INPUT

    //ERROR: Unknown command: 'DISCONNECT 2'
    //ERROR: INVALID INPUT
    flags.elements[WIFI_MOD_ERR_IDX].value = 1;

    while (!serial_rb_empty(&rxbuf))
        serial_rb_read(&rxbuf);   // clear receive buffer

    delay_ms(2000);

    while (!serial_rb_empty(&rxbuf))
        serial_rb_read(&rxbuf);   // clear receive buffer

    return 0;
}
#endif
