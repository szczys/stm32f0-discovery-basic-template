/*
 * usart.h
 */

#ifndef USART_H_
#define USART_H_

#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "serial_rb.h"

#define BUFFERED

#ifdef BUFFERED
#define RXBUFFSIZE	64
#define TXBUFFSIZE	64
extern serial_rb rxbuf;
extern serial_rb txbuf;
#endif

extern volatile uint8_t rx_lines_count;
extern volatile uint8_t tx_lines_count;

void Usart1Init(void);
void Usart1Put(uint8_t c);
uint8_t Usart1Get(void);
ErrorStatus usart_readline(char *dst, uint8_t len);

#endif /* USART_H_ */
