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

enum {NO_FREE_MEMORY_IDX, LOW_BATTERY_IDX, WIFI_MOD_ERR_IDX, BORED_IDX};
extern list_t flags;

enum {TOGGLE_LEDS_IDX, REALTIME_MSG_IDX};
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
