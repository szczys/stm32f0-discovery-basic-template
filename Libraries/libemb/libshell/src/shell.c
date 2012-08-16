/*
 * This file is part of the libemb project.
 *
 * Copyright (C) 2011 Stefan Wendler <sw@kaltpost.de>
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

#include "shell.h"

int shell_str_len(char *str)
{
     int i = 0;

     while(str[i++] != 0);

     return i - 1;
}

int shell_str_cmp(char *str1, char *str2, int len1, int len2)
{
     int i;

     if(len1 > len2) return 1;

     for(i = 0; i < len1; i++) {
          if(str1[i] != str2[i]) return 2;
     }

     // make sure we matched a whole command, and not only
     // a containing substring ...
     if(len2 > len1 && str2[i] != ' ') {
          return 2;
     }

     return 0;
}

int shell_parse_int(char *str)
{

     int val = 0;
     int i   = 0;

     char c;

     while((c = str[i++]) != 0) {

          if(c < '0' || c > '9') break;

          val = val * 10 + (c - '0');
     }

     return val;
}

int shell_arg_parser(char *cmd_line, int len,  shell_cmd_args *args)
{
     int i;
     int j;
     int spos = 0;
     int argc = 0;

     for(i = 0; i < len; i++) {
          // to many arguments
          if(argc > SHELL_MAX_ARGS) return 1;

          if(cmd_line[i] == ' ' || i == len - 1) {
               // catch last argument ...
               if(i == len - 1) i++;

               // ignore first since it is the cmd itself
               if(spos == 0) {
                    spos = i;
               } else {
                    // argument value to long
                    if(i - spos > SHELL_MAX_ARG_LEN) return 2;

                    for(j = 0; j < i - spos - 1; j++) {
                         args->args[argc].val[j] = cmd_line[spos + 1 + j];
                    }
                    args->args[argc++].val[j] = 0;
                    spos = i;
               }
          }
     }

     args->count = argc;

     return 0;
}

int shell_process_cmds(shell_cmds *cmds, char *cmd_line)
{
     int i;
     int ret;
     int cmd_len;
     int cmd_line_len;

     shell_cmd_args args;

     for(i = 0; i < cmds->count; i++) {

          cmd_line_len 	= shell_str_len(cmd_line);
          cmd_len 		= shell_str_len((char *)(cmds->cmds[i].cmd));

          if(shell_str_cmp((char *)(cmds->cmds[i].cmd), cmd_line, cmd_len, cmd_line_len) == 0) {
               ret = shell_arg_parser(cmd_line, cmd_line_len, &args);

               if(ret == 1)
                    return SHELL_PROCESS_ERR_ARGS_MAX;
               if(ret == 2)
                    return SHELL_PROCESS_ERR_ARGS_LEN;

               return (cmds->cmds[i].func)(&args);
          }
     }

     return SHELL_PROCESS_ERR_CMD_UNKN;
}
