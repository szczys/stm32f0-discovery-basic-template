/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef WIFI_H
#define WIFI_H

#include "stm32f0xx.h"
#include "usart.h"
#include <stdbool.h>

#define RCC_AHBPeriph_WIFI_RESET RCC_AHBPeriph_GPIOA
#define WIFI_MOD_RESET_PORT GPIOA
#define WIFI_MOD_RESET_PIN GPIO_Pin_8

extern int8_t cid;

/* Each connection has a unique, single-digit hexadecimal
   value (0 to F), for the CID. */
extern char CID[];

// Configure input/output pins for the WiFi module
void wifi_intit_pins();

// Reset the WiFi module via reset line
void reset_wifi_module();

// Init in Accesspoint Mode
void wifi_init_ap_mode();

uint8_t get_wifi_msg(char *buffer, int bufferLength, int8_t *cid);

#endif
