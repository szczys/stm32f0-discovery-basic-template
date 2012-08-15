/*
 * microcli.c
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

#include "../inc/microcli.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef PC
#define printf printf
#else
#include "main.h"
#include "stm32f0_discovery.h"
//#include "conf.h"
//#include "util.h"
//#define printf m_print
//#ifdef LCD_CONNECTED
//#include "display.h"
//#elif defined(DOOR_SIGN_CONNECTED)
// #include "door_sign.h"
//#endif
#endif

#define OUTPUT_COUNT 8
char outputs[OUTPUT_COUNT][10] = {{"LED4"}, {"LED3"}, {"LED5"}, {"LED6"},
    {"RELAIS1"}, {"RELAIS2"}, {"OUT_12V_1"}, {"OUT_12V_2"}
};

void picolInitParser(struct picolParser *p, char *text)
{
    p->text = p->p = text;
    p->len = strlen(text);
    p->start = 0; p->end = 0; p->insidequote = 0;
    p->type = PT_EOL;
}

int picolParseSep(struct picolParser *p)
{
    p->start = p->p;
    while (*p->p == ' ' || *p->p == '\t' || *p->p == '\n' || *p->p == '\r')
    {
        p->p++; p->len--;
    }
    p->end = p->p - 1;
    p->type = PT_SEP;
    return PICOL_OK;
}

int picolParseEol(struct picolParser *p)
{
    p->start = p->p;
    while (*p->p == ' ' || *p->p == '\t' || *p->p == '\n' || *p->p == '\r' ||
            *p->p == ';')
    {
        p->p++; p->len--;
    }
    p->end = p->p - 1;
    p->type = PT_EOL;
    return PICOL_OK;
}

int picolParseCommand(struct picolParser *p)
{
    int level = 1;
    int blevel = 0;
    p->start = ++p->p; p->len--;
    while (1)
    {
        if (p->len == 0)
        {
            break;
        }
        else if (*p->p == '[' && blevel == 0)
        {
            level++;
        }
        else if (*p->p == ']' && blevel == 0)
        {
            if (!--level) break;
        }
        else if (*p->p == '\\')
        {
            p->p++; p->len--;
        }
        else if (*p->p == '{')
        {
            blevel++;
        }
        else if (*p->p == '}')
        {
            if (blevel != 0) blevel--;
        }
        p->p++; p->len--;
    }
    p->end = p->p - 1;
    p->type = PT_CMD;
    if (*p->p == ']')
    {
        p->p++; p->len--;
    }
    return PICOL_OK;
}

int picolParseVar(struct picolParser *p)
{
    p->start = ++p->p; p->len--; /* skip the $ */
    while (1)
    {
        if ((*p->p >= 'a' && *p->p <= 'z') || (*p->p >= 'A' && *p->p <= 'Z') ||
                (*p->p >= '0' && *p->p <= '9') || *p->p == '_')
        {
            p->p++; p->len--; continue;
        }
        break;
    }
    if (p->start == p->p)   /* It's just a single char string "$" */
    {
        p->start = p->end = p->p - 1;
        p->type = PT_STR;
    }
    else
    {
        p->end = p->p - 1;
        p->type = PT_VAR;
    }
    return PICOL_OK;
}

int picolParseBrace(struct picolParser *p)
{
    int level = 1;
    p->start = ++p->p; p->len--;
    while (1)
    {
        if (p->len >= 2 && *p->p == '\\')
        {
            p->p++; p->len--;
        }
        else if (p->len == 0 || *p->p == '}')
        {
            level--;
            if (level == 0 || p->len == 0)
            {
                p->end = p->p - 1;
                if (p->len)
                {
                    p->p++; p->len--; /* Skip final closed brace */
                }
                p->type = PT_STR;
                return PICOL_OK;
            }
        }
        else if (*p->p == '{')
            level++;
        p->p++; p->len--;
    }
    return PICOL_OK; /* unreached */
}

int picolParseString(struct picolParser *p)
{
    int newword = (p->type == PT_SEP || p->type == PT_EOL || p->type == PT_STR);
    if (newword && *p->p == '{') return picolParseBrace(p);
    else if (newword && *p->p == '"')
    {
        p->insidequote = 1;
        p->p++; p->len--;
    }
    p->start = p->p;
    while (1)
    {
        if (p->len == 0)
        {
            p->end = p->p - 1;
            p->type = PT_ESC;
            return PICOL_OK;
        }
        switch (*p->p)
        {
        case '\\':
            if (p->len >= 2)
            {
                p->p++; p->len--;
            }
            break;
        case '$': case '[':
            p->end = p->p - 1;
            p->type = PT_ESC;
            return PICOL_OK;
        case ' ': case '\t': case '\n': case '\r': case ';':
            if (!p->insidequote)
            {
                p->end = p->p - 1;
                p->type = PT_ESC;
                return PICOL_OK;
            }
            break;
        case '"':
            if (p->insidequote)
            {
                p->end = p->p - 1;
                p->type = PT_ESC;
                p->p++; p->len--;
                p->insidequote = 0;
                return PICOL_OK;
            }
            break;
        }
        p->p++; p->len--;
    }
    return PICOL_OK; /* unreached */
}

int picolParseComment(struct picolParser *p)
{
    while (p->len && *p->p != '\n')
    {
        p->p++; p->len--;
    }
    return PICOL_OK;
}

int picolGetToken(struct picolParser *p)
{
    while (1)
    {
        if (!p->len)
        {
            if (p->type != PT_EOL && p->type != PT_EOF)
                p->type = PT_EOL;
            else
                p->type = PT_EOF;
            return PICOL_OK;
        }
        switch (*p->p)
        {
        case ' ': case '\t': case '\r':
            if (p->insidequote) return picolParseString(p);
            return picolParseSep(p);
        case '\n': case ';':
            if (p->insidequote) return picolParseString(p);
            return picolParseEol(p);
        case '[':
            return picolParseCommand(p);
        case '$':
            return picolParseVar(p);
        case '#':
            if (p->type == PT_EOL)
            {
                picolParseComment(p);
                continue;
            }
            return picolParseString(p);
        default:
            return picolParseString(p);
        }
    }
    return PICOL_OK; /* unreached */
}

void picolInitInterp(struct picolInterp *i)
{
    i->level = 0;
    i->callframe = malloc(sizeof(struct picolCallFrame));
    i->callframe->vars = NULL;
    i->callframe->parent = NULL;
    i->commands = NULL;
    i->result = strdup("");
}

void picolSetResult(struct picolInterp *i, char *s)
{
    free(i->result);
    i->result = strdup(s);
}

struct picolVar *picolGetVar(struct picolInterp *i, char *name)
{
    struct picolVar *v = i->callframe->vars;
    while (v)
    {
        if (strcmp(v->name, name) == 0) return v;
        v = v->next;
    }
    return NULL;
}

int picolSetVar(struct picolInterp *i, char *name, char *val)
{
    struct picolVar *v = picolGetVar(i, name);
    if (v)
    {
        free(v->val);
        v->val = strdup(val);
    }
    else
    {
        v = malloc(sizeof(*v));
        v->name = strdup(name);
        v->val = strdup(val);
        v->next = i->callframe->vars;
        i->callframe->vars = v;
    }
    return PICOL_OK;
}

struct picolCmd *picolGetCommand(struct picolInterp *i, char *name)
{
    struct picolCmd *c = i->commands;
    while (c)
    {
        if (strcmp(c->name, name) == 0) return c;
        c = c->next;
    }
    return NULL;
}

int picolRegisterCommand(struct picolInterp *i, char *name, picolCmdFunc f, void *privdata, char *help)
{
    struct picolCmd *c = picolGetCommand(i, name);
    if (c)
    {
        snprintf(strbuf, STR_BUFF_SIZE, "Command '%s' already defined", name);
        picolSetResult(i, strbuf);
        return PICOL_ERR;
    }
    c = malloc(sizeof(*c));
    c->name = strdup(name);
    c->func = f;
    c->privdata = privdata;
    if (*help != '\0')
        c->help = strdup(help);
    else
        c->help = '\0';
    c->next = i->commands;
    i->commands = c;
    return PICOL_OK;
}

/* EVAL! */
int picolEval(struct picolInterp *i, char *t)
{
    struct picolParser p;
    int argc = 0, j;
    char **argv = NULL;
    int retcode = PICOL_OK;
    picolSetResult(i, "");
    picolInitParser(&p, t);
    while (1)
    {
        char *t;
        int tlen;
        int prevtype = p.type;
        picolGetToken(&p);
        if (p.type == PT_EOF) break;
        tlen = p.end - p.start + 1;
        if (tlen < 0) tlen = 0;
        t = malloc(tlen + 1);
        memcpy(t, p.start, tlen);
        t[tlen] = '\0';
        if (p.type == PT_VAR)
        {
            struct picolVar *v = picolGetVar(i, t);
            if (!v)
            {
                snprintf(strbuf, STR_BUFF_SIZE, "No such variable '%s'", t);
                free(t);
                picolSetResult(i, strbuf);
                retcode = PICOL_ERR;
                goto err;
            }
            free(t);
            t = strdup(v->val);
        }
        else if (p.type == PT_CMD)
        {
            retcode = picolEval(i, t);
            free(t);
            if (retcode != PICOL_OK) goto err;
            t = strdup(i->result);
        }
        else if (p.type == PT_ESC)
        {
            /* XXX: escape handling missing! */
        }
        else if (p.type == PT_SEP)
        {
            prevtype = p.type;
            free(t);
            continue;
        }
        /* We have a complete command + args. Call it! */
        if (p.type == PT_EOL)
        {
            struct picolCmd *c;
            free(t);
            prevtype = p.type;
            if (argc)
            {
                if ((c = picolGetCommand(i, argv[0])) == NULL)
                {
                    snprintf(strbuf, STR_BUFF_SIZE, "No such command '%s'", argv[0]);
                    picolSetResult(i, strbuf);
                    retcode = PICOL_ERR;
                    goto err;
                }
                retcode = c->func(i, argc, argv, c->privdata);
                if (retcode != PICOL_OK) goto err;
            }
            /* Prepare for the next command */
            for (j = 0; j < argc; j++) free(argv[j]);
            free(argv);
            argv = NULL;
            argc = 0;
            continue;
        }
        /* We have a new token, append to the previous or as new arg? */
        if (prevtype == PT_SEP || prevtype == PT_EOL)
        {
            argv = realloc(argv, sizeof(char*) * (argc + 1));
            argv[argc] = t;
            argc++;
        }
        else     /* Interpolation */
        {
            int oldlen = strlen(argv[argc - 1]), tlen = strlen(t);
            argv[argc - 1] = realloc(argv[argc - 1], oldlen + tlen + 1);
            memcpy(argv[argc - 1] + oldlen, t, tlen);
            argv[argc - 1][oldlen + tlen] = '\0';
            free(t);
        }
        prevtype = p.type;
    }
err:
    for (j = 0; j < argc; j++) free(argv[j]);
    free(argv);
    return retcode;
}

/* ACTUAL COMMANDS! */
int picolArityErr(struct picolInterp *i, char *name)
{
    snprintf(strbuf, STR_BUFF_SIZE, "Wrong number of args for %s", name);
    picolSetResult(i, strbuf);
    return PICOL_ERR;
}

int picolCommandMath(struct picolInterp *i, int argc, char **argv, void *pd)
{
    int a, b, c;
    if (argc != 3) return picolArityErr(i, argv[0]);
    a = atoi(argv[1]); b = atoi(argv[2]);
    if (argv[0][0] == '+') c = a + b;
    else if (argv[0][0] == '-') c = a - b;
    else if (argv[0][0] == '*') c = a * b;
    else if (argv[0][0] == '/') c = a / b;
    else if (argv[0][0] == '>' && argv[0][1] == '\0') c = a > b;
    else if (argv[0][0] == '>' && argv[0][1] == '=') c = a >= b;
    else if (argv[0][0] == '<' && argv[0][1] == '\0') c = a < b;
    else if (argv[0][0] == '<' && argv[0][1] == '=') c = a <= b;
    else if (argv[0][0] == '=' && argv[0][1] == '=') c = a == b;
    else if (argv[0][0] == '!' && argv[0][1] == '=') c = a != b;
    else c = 0; /* I hate warnings */
    snprintf(strbuf, STR_BUFF_SIZE, "%d", c);
    picolSetResult(i, strbuf);
    return PICOL_OK;
}

int picolCommandSet(struct picolInterp *i, int argc, char **argv, void *pd)
{
    if (argc != 3) return picolArityErr(i, argv[0]);
    picolSetVar(i, argv[1], argv[2]);
    picolSetResult(i, argv[2]);
    return PICOL_OK;
}

int picolCommandPuts(struct picolInterp *i, int argc, char **argv, void *pd)
{
    if (argc != 2) return picolArityErr(i, argv[0]);
    printf("%s\n", argv[1]);
    return PICOL_OK;
}

int picolCommandIf(struct picolInterp *i, int argc, char **argv, void *pd)
{
    int retcode;
    if (argc != 3 && argc != 5) return picolArityErr(i, argv[0]);
    if ((retcode = picolEval(i, argv[1])) != PICOL_OK) return retcode;
    if (atoi(i->result)) return picolEval(i, argv[2]);
    else if (argc == 5) return picolEval(i, argv[4]);
    return PICOL_OK;
}

int picolCommandWhile(struct picolInterp *i, int argc, char **argv, void *pd)
{
    if (argc != 3) return picolArityErr(i, argv[0]);
    while (1)
    {
        int retcode = picolEval(i, argv[1]);
        if (retcode != PICOL_OK) return retcode;
        if (atoi(i->result))
        {
            if ((retcode = picolEval(i, argv[2])) == PICOL_CONTINUE) continue;
            else if (retcode == PICOL_OK) continue;
            else if (retcode == PICOL_BREAK) return PICOL_OK;
            else return retcode;
        }
        else
        {
            return PICOL_OK;
        }
    }
}

int picolCommandRetCodes(struct picolInterp *i, int argc, char **argv, void *pd)
{
    if (argc != 1) return picolArityErr(i, argv[0]);
    if (strcmp(argv[0], "break") == 0) return PICOL_BREAK;
    else if (strcmp(argv[0], "continue") == 0) return PICOL_CONTINUE;
    return PICOL_OK;
}

void picolDropCallFrame(struct picolInterp *i)
{
    struct picolCallFrame *cf = i->callframe;
    struct picolVar *v = cf->vars, *t;
    while (v)
    {
        t = v->next;
        free(v->name);
        free(v->val);
        free(v);
        v = t;
    }
    i->callframe = cf->parent;
    free(cf);
}

int picolCommandCallProc(struct picolInterp *i, int argc, char **argv, void *pd)
{
    char **x = pd, *alist = x[0], *body = x[1], *p = strdup(alist), *tofree;
    struct picolCallFrame *cf = malloc(sizeof(*cf));
    int arity = 0, done = 0, errcode = PICOL_OK;
    cf->vars = NULL;
    cf->parent = i->callframe;
    i->callframe = cf;
    tofree = p;
    while (1)
    {
        char *start = p;
        while (*p != ' ' && *p != '\0') p++;
        if (*p != '\0' && p == start)
        {
            p++; continue;
        }
        if (p == start) break;
        if (*p == '\0') done = 1; else *p = '\0';
        if (++arity > argc - 1) goto arityerr;
        picolSetVar(i, start, argv[arity]);
        p++;
        if (done) break;
    }
    free(tofree);
    if (arity != argc - 1) goto arityerr;
    errcode = picolEval(i, body);
    if (errcode == PICOL_RETURN) errcode = PICOL_OK;
    picolDropCallFrame(i); /* remove the called proc callframe */
    return errcode;
arityerr:
    snprintf(strbuf, STR_BUFF_SIZE, "Proc '%s' called with wrong arg num", argv[0]);
    picolSetResult(i, strbuf);
    picolDropCallFrame(i); /* remove the called proc callframe */
    return PICOL_ERR;
}

int picolCommandProc(struct picolInterp *i, int argc, char **argv, void *pd)
{
    char **procdata = malloc(sizeof(char*) * 2);
    if (argc != 4) return picolArityErr(i, argv[0]);
    procdata[0] = strdup(argv[2]); /* arguments list */
    procdata[1] = strdup(argv[3]); /* procedure body */
    return picolRegisterCommand(i, argv[1], picolCommandCallProc, procdata, "registered script function");
}

int picolCommandReturn(struct picolInterp *i, int argc, char **argv, void *pd)
{
    if (argc != 1 && argc != 2) return picolArityErr(i, argv[0]);
    picolSetResult(i, (argc == 2) ? argv[1] : "");
    return PICOL_RETURN;
}

int microcliCommandList(struct picolInterp *i, int argc, char **argv, void *pd)
{
    if (argc != 2) return picolArityErr(i, argv[0]);
    if (strcmp(argv[1], "input") == 0)
    {
        printf("BUTTON1\n");
#ifdef DEVICE_STM32F4_WIFI
        printf("IN_12V_1\n");
        printf("IN_12V_2\n");
#endif
#ifdef DOOR_SIGN_CONNECTED
        printf("door_sign\n");
#endif
        return PICOL_OK;
    }
    else if (strcmp(argv[1], "output") == 0)
    {
        printf(
#ifdef DEVICE_DISCOVERY_BLUETOOTH
               "LED4\n"
#endif
               "LED3\n"
               "LED5\n"
               "LED6\n"
#ifdef DEVICE_STM32F4_WIFI
               "RELAIS1\n"
               "RELAIS2\n"
               "OUT_12V_1\n"
               "OUT_12V_2\n"
#endif
#ifdef LCD_CONNECTED
               "LCD1\n"
#endif
#ifdef DOOR_SIGN_CONNECTED
                "door_sign\n"
#endif
              );
        return PICOL_OK;
    }
    else if (strcmp(argv[1], "commands") == 0)
    {
        struct picolCmd *c = i->commands;
        while (c)
        {
            if (c->help != '\0')
                printf("%s %s\n", c->name, c->help);
            c = c->next;
        }
        return PICOL_OK;
    }
    else if (strcmp(argv[1], "options") == 0)
    {
        printf("TOGGLE_LEDS\n"
               "LIVE_MESSAGES\n");
        return PICOL_OK;
    }
    else if (strcmp(argv[1], "flags") == 0)
    {
        printf("LOW_MEMORY\n"
               "LOW_BATTERY\n"
               "I'M_BORED\n");
        return PICOL_OK;
    }
    return picolArityErr(i, argv[0]);
}

int microcliCommandDevInfo(struct picolInterp *i, int argc, char **argv, void *pd)
{
    if (argc != 1) return picolArityErr(i, argv[0]);
    printf("Device Type: F0-Discovery Demo\n"
           "Hardware Revision: 1.0\n"
           "Firmware Version: 1.8\n"
           "URL: www.jann.cc\n");
    return PICOL_OK;

}

// these commands won't work on the PC
#ifndef PC
int microcliCommandEnable(struct picolInterp *i, int argc, char **argv, void *pd)
{
    if ((argc < 2) || (argc > 3)) return picolArityErr(i, argv[0]);
    if (strcmp(argv[1], "LIVE_MESSAGES") == 0)
    {
        //enable_messages = true;
        return PICOL_OK;
    }
    else if (strcmp(argv[1], "TOGGLE_LEDS") == 0)
    {
        //toggle_leds = true;
        return PICOL_OK;
    }
#ifdef DOOR_SIGN_CONNECTED
    else if (strcmp(argv[1], "door_sign") == 0)
    {
        if(argc == 2)
        {
            snprintf(strbuf, STR_BUFF_SIZE, "Missing argument for door_sign, "
                    "usage: enable door_sign free|occupied");
                picolSetResult(i, strbuf);
                return PICOL_ERR;
        }
        else if (argc == 3)
        {
            if (strcmp(argv[1], "door_sign") == 0)
            {
                if (strcmp(argv[2], "free") == 0)
                {
                    //toggle_leds = false;
                    //STM_EVAL_LEDOn(LED6);
                    //STM_EVAL_LEDOff(LED3);
                    //STM_EVAL_LEDOff(LED5);
                    //door_sign_set(DOOR_SIGN_FREE);
                    return PICOL_OK;
                }
                else if (strcmp(argv[2], "occupied") == 0)
                {
                    //toggle_leds = false;
                    //STM_EVAL_LEDOn(LED3);
                    //STM_EVAL_LEDOff(LED6);
                    //STM_EVAL_LEDOff(LED5);
                    //door_sign_set(DOOR_SIGN_OCCUPIED);
                    return PICOL_OK;
                }
            }

        }
    }
#endif
    int n = 0;
    for (n = 0; n < OUTPUT_COUNT; n++)
    {
        if (strcmp(argv[1], outputs[n]) == 0)
        {
            STM_EVAL_LEDOn(n);
            return PICOL_OK;
        }

    }
    return picolArityErr(i, argv[0]);
}

int microcliCommandDisable(struct picolInterp *i, int argc, char **argv, void *pd)
{
    if (argc != 2) return picolArityErr(i, argv[0]);
    if (strcmp(argv[1], "LIVE_MESSAGES") == 0)
    {
        //enable_messages = false;
        return PICOL_OK;
    }
    else if (strcmp(argv[1], "TOGGLE_LEDS") == 0)
    {
        //toggle_leds = false;
        return PICOL_OK;
    }
#ifdef DOOR_SIGN_CONNECTED
    else if (strcmp(argv[1], "door_sign") == 0)
    {
        toggle_leds = true;
        STM_EVAL_LEDOff(LED3);
        STM_EVAL_LEDOff(LED5);
        STM_EVAL_LEDOff(LED6);
        door_sign_set(DOOR_SIGN_OFF);
        return PICOL_OK;
    }
#endif
    int n = 0;
    for (n = 0; n < OUTPUT_COUNT; n++)
    {
        if (strcmp(argv[1], outputs[n]) == 0)
        {
            STM_EVAL_LEDOff(n);
            return PICOL_OK;
        }

    }
    return picolArityErr(i, argv[0]);
}

int microcliCommandToggle(struct picolInterp *i, int argc, char **argv, void *pd)
{
    if (argc != 2) return picolArityErr(i, argv[0]);
    if (strcmp(argv[1], "LIVE_MESSAGES") == 0)
    {
        //enable_messages = false;
        return PICOL_OK;
    }
    else if (strcmp(argv[1], "TOGGLE_LEDS") == 0)
    {
        //toggle_leds = false;
        return PICOL_OK;
    }
    int n = 0;
    for (n = 0; n < OUTPUT_COUNT; n++)
    {
        if (strcmp(argv[1], outputs[n]) == 0)
        {
            STM_EVAL_LEDToggle(n);
            return PICOL_OK;
        }

    }
    return picolArityErr(i, argv[0]);
}

int microcliCommandGet(struct picolInterp *i, int argc, char **argv, void *pd)
{
    if (argc != 2) return picolArityErr(i, argv[0]);
    if (strcmp(argv[1], "LIVE_MESSAGES") == 0)
    {
//        if (enable_messages)
//            picolSetResult(i, "true");
//        else
//            picolSetResult(i, "false");
        return PICOL_OK;
    }
    else if (strcmp(argv[1], "TOGGLE_LEDS") == 0)
    {

//        if (toggle_leds)
//            picolSetResult(i, "true");
//        else
//            picolSetResult(i, "false");
        return PICOL_OK;
    }
    else if (strcmp(argv[1], "LOW_MEMORY") == 0)
    {
        picolSetResult(i, "false");
        return PICOL_OK;
    }
    else if (strcmp(argv[1], "LOW_BATTERY") == 0)
    {
        picolSetResult(i, "false");
        return PICOL_OK;
    }
    else if (strcmp(argv[1], "I'M_BORED") == 0)
    {
        picolSetResult(i, "true");
        return PICOL_OK;
    }
    else if (strcmp(argv[1], "BUTTON1") == 0)
    {
        // TODO: use inputs[] array, like outputs[]
        if (STM_EVAL_PBGetState(BUTTON_USER))
            picolSetResult(i, "pushed");
        else
            picolSetResult(i, "released");
        return PICOL_OK;
    }
#ifdef DEVICE_STM32F4_WIFI
    else if (strcmp(argv[1], "IN_12V_1") == 0)
    {
        // TODO: use inputs[] array, like outputs[]
        if (STM_EVAL_PBGetState(IN_12V_1))
            picolSetResult(i, "on");
        else
            picolSetResult(i, "off");
        return PICOL_OK;
    }
    else if (strcmp(argv[1], "IN_12V_2") == 0)
    {
        // TODO: use inputs[] array, like outputs[]
        if (STM_EVAL_PBGetState(IN_12V_2))
            picolSetResult(i, "on");
        else
            picolSetResult(i, "off");
        return PICOL_OK;
    }
#endif
#ifdef DOOR_SIGN_CONNECTED
    else if (strcmp(argv[1], "door_sign") == 0)
    {
        switch (door_sign_get())
        {
        case DOOR_SIGN_FREE:
            picolSetResult(i, "free");
            break;
        case DOOR_SIGN_OCCUPIED:
            picolSetResult(i, "occupied");
            break;
        case DOOR_SIGN_OFF:
            picolSetResult(i, "off");
            break;
        }

        return PICOL_OK;
    }
#endif
#ifdef LCD_CONNECTED
    else if (strcmp(argv[1], "LCD1") == 0)
    {
        printf("%s\n%s\n", current_lcd_text[0], current_lcd_text[1]);
        return PICOL_OK;
    }
#endif
    int n = 0;
    for (n = 0; n < OUTPUT_COUNT; n++)
    {
        if (strcmp(argv[1], outputs[n]) == 0)
        {
//            if (GPIO_ReadOutputDataBit(GPIO_PORT[n], GPIO_PIN[n]))
//                picolSetResult(i, "on");
//            else
//                picolSetResult(i, "off");
            return PICOL_OK;
        }

    }
    return picolArityErr(i, argv[0]);
}

int microcliCommandWaitFor(struct picolInterp *i, int argc, char **argv, void *pd)
{
    if (argc != 2) return picolArityErr(i, argv[0]);
    if (strcmp(argv[1], "BUTTON1") == 0)
    {
        /* Waiting User Button is pressed */
//        while (UserButtonPressed == 0x00)
//            {}

//        /* Waiting User Button is Released */
//        while (STM_EVAL_PBGetState(BUTTON_USER) == Bit_SET)
//            {}
//        UserButtonPressed = 0x00;

        return PICOL_OK;
    }
    return picolArityErr(i, argv[0]);
}

int microcliCommandPing(struct picolInterp *i, int argc, char **argv, void *pd)
{
    printf("pong\n");
    return PICOL_OK;
}

int microcliCommandAbout(struct picolInterp *i, int argc, char **argv, void *pd)
{
    //send_welcome_message();
    return PICOL_OK;
}
#ifdef LCD_CONNECTED
int microcliCommandLCD(struct picolInterp *i, int argc, char **argv, void *pd)
{
    if ((argc == 2) && (strcmp(argv[1], "clear") == 0))
    {
        display_clear();
        return PICOL_OK;
    }
    else if (argc == 4)
    {
        display_puts(argv[3], atoi(argv[1]), atoi(argv[2]));
        return PICOL_OK;
    }
    return picolArityErr(i, argv[0]);
}
#endif
#endif

void picolRegisterCoreCommands(struct picolInterp *i)
{
    int j; char *name[] = {"+", "-", "*", "/", ">", ">=", "<", "<=", "==", "!="};
    for (j = 0; j < (int)(sizeof(name) / sizeof(char*)); j++)
        picolRegisterCommand(i, name[j], picolCommandMath, NULL, "");
    picolRegisterCommand(i, "set", picolCommandSet, NULL, "--> set a variable");
    picolRegisterCommand(i, "puts", picolCommandPuts, NULL, "--> print a string");
    picolRegisterCommand(i, "if", picolCommandIf, NULL, "--> if condition");
    picolRegisterCommand(i, "while", picolCommandWhile, NULL, "--> loop");
    picolRegisterCommand(i, "break", picolCommandRetCodes, NULL, "--> break");
    picolRegisterCommand(i, "continue", picolCommandRetCodes, NULL, "--> continue");
    picolRegisterCommand(i, "proc", picolCommandProc, NULL, "--> define a function");
    picolRegisterCommand(i, "return", picolCommandReturn, NULL, "--> return command");
    picolRegisterCommand(i, "list", microcliCommandList, NULL, "--> list input/output/commands/options/flags");
    picolRegisterCommand(i, "deviceinfo", microcliCommandDevInfo, NULL, "--> Show device information");
#ifndef PC
    // these commands won't work on the PC
    picolRegisterCommand(i, "enable", microcliCommandEnable, NULL, "--> enable an LED or an option");
    picolRegisterCommand(i, "disable", microcliCommandDisable, NULL, "--> disable an LED or an option");
    picolRegisterCommand(i, "toggle", microcliCommandToggle, NULL, "--> toggle an LED");
    picolRegisterCommand(i, "get", microcliCommandGet, NULL, "--> get LED0 or GET BUTTON1");
    picolRegisterCommand(i, "waitfor", microcliCommandWaitFor, NULL, "--> waitfor BUTTON1, sleep until BUTTON1 is pressed");
    picolRegisterCommand(i, "about", microcliCommandPing, NULL, "--> show a welcome message");
    picolRegisterCommand(i, "ping", microcliCommandPing, NULL, "--> send pong back");
#ifdef LCD_CONNECTED
    picolRegisterCommand(i, "lcd", microcliCommandLCD, NULL, "--> lcd 0 0 \"Hello!\"");
#endif
#endif
}

#ifdef PC
int main(int argc, char **argv)
{
    struct picolInterp interp;
    picolInitInterp(&interp);
    picolRegisterCoreCommands(&interp);
    printf("MicroCLI 0.1.0 (2012-08-15)\n"
           "Type \"copyright\", \"credits\" or \"license\" for more information.\n"
           "MicroCLI -- An embedded command line interpreter.\n"
           "about            -> Introduction and overview of TurckCLI's features.\n"
           "list commands    -> Get a list of the builtin commands.\n"
           "help 'commandxy' -> Details about 'commandxy'.\n");
    if (argc == 1)
    {
        while (1)
        {
            char clibuf[1024];
            int retcode;
            printf("microcli> "); fflush(stdout);
            if (fgets(clibuf, 1024, stdin) == NULL) return 0;
            retcode = picolEval(&interp, clibuf);
            if (interp.result[0] != '\0')
                printf("[%d] %s\n", retcode, interp.result);
        }
    }
    else if (argc == 2)
    {
        char buf[1024 * 16];
        FILE *fp = fopen(argv[1], "r");
        if (!fp)
        {
            perror("open"); exit(1);
        }
        buf[fread(buf, 1, 1024 * 16, fp)] = '\0';
        fclose(fp);
        if (picolEval(&interp, buf) != PICOL_OK) printf("%s\n", interp.result);
    }
    return 0;
}
#endif

