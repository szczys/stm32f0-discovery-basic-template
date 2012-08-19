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

#ifndef WIFI_H
#define WIFI_H

#include "stm32f0xx.h"
#include "usart.h"
#include <stdbool.h>

#define RCC_AHBPeriph_WIFI_RESET RCC_AHBPeriph_GPIOA
#define WIFI_MOD_RESET_PORT GPIOA
#define WIFI_MOD_RESET_PIN GPIO_Pin_8

enum {DISCONNECTED, CONNECTED};

typedef struct
{
    int8_t      cid;
    char        ip[16];
    char        port[10];
    uint8_t     state; // CONNECTED, DISCONNECTED

} wifi_connection_t;

wifi_connection_t wifi_connections[16];

extern int8_t cid;

/* Each connection has a unique, single-digit hexadecimal
   value (0 to F), for the CID. */
extern char CID[];

// Configure input/output pins for the WiFi module
void wifi_intit_pins();

// Reset the WiFi module via reset line
void wifi_module_reset();

// Init in Accesspoint Mode
void wifi_init_ap_mode();

uint8_t wifi_get_msg(char *buffer, int bufferLength, int8_t *cid);

void wifi_init_connections();

#endif
