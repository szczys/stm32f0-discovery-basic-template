/*
 * microcli.h
 *
 * Licence: BSD
 *
 * Based on picol <http://antirez.com/page/picol.html>
 * Tcl in ~ 500 lines of code by Salvatore antirez Sanfilippo.
 *
 * Copyright (C) 2012, Christian Jann <christian.jann@ymail.com>
 *
 * To compile microcli for the PC run: gcc -DPC -O2 -Wall -o microcli  microcli.c
 * and than you can run the interactive interpreter: ./microcli
 *
 * You can use Cygwin to install gcc on Windows.
 *
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MICROCLI_H
#define MICROCLI_H

enum {PICOL_OK, PICOL_ERR, PICOL_RETURN, PICOL_BREAK, PICOL_CONTINUE};
enum {PT_ESC, PT_STR, PT_CMD, PT_VAR, PT_SEP, PT_EOL, PT_EOF};

#define STR_BUFF_SIZE 150
char strbuf[STR_BUFF_SIZE];

struct picolParser
{
    char *text;
    char *p; /* current text position */
    int len; /* remaining length */
    char *start; /* token start */
    char *end; /* token end */
    int type; /* token type, PT_... */
    int insidequote; /* True if inside " " */
};

struct picolVar
{
    char *name, *val;
    struct picolVar *next;
};

struct picolInterp; /* forward declaration */
typedef int (*picolCmdFunc)(struct picolInterp *i, int argc, char **argv, void *privdata);

struct picolCmd
{
    char *name;
    picolCmdFunc func;
    void *privdata;
    char *help;
    struct picolCmd *next;
};

struct picolCallFrame
{
    struct picolVar *vars;
    struct picolCallFrame *parent; /* parent is NULL at top level */
};

struct picolInterp
{
    int level; /* Level of nesting */
    struct picolCallFrame *callframe;
    struct picolCmd *commands;
    char *result;
};

void picolInitInterp(struct picolInterp *i);
void picolRegisterCoreCommands(struct picolInterp *i);
int picolEval(struct picolInterp *i, char *t) ;

#endif
