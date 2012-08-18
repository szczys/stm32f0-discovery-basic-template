/*
 * usart.h
 */

#ifndef USART_H_
#define USART_H_

#include "conf.h"
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "serial_rb.h"

#ifdef BUFFERED
extern serial_rb rxbuf;
extern serial_rb txbuf;
#endif

extern volatile uint8_t rx_lines_count;
extern volatile uint8_t tx_lines_count;
extern uint32_t current_baud;

void usart_1_init(void);
void usart_1_put(uint8_t c);
uint8_t usart_1_get(void);
void usart_1_change_baud(uint32_t USART_BaudRate);
uint8_t usart_readline(char *dst, uint8_t len);

#endif /* USART_H_ */
