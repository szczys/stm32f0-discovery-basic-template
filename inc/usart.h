/*
 * usart.h
 */

#ifndef USART_H_
#define USART_H_

#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "buffer.h"

#define BUFFERED

#ifdef BUFFERED
extern volatile FIFO_TypeDef U1Rx, U1Tx;
extern volatile uint8_t rx_buffer_lines_count;
extern volatile uint8_t tx_buffer_lines_count;
#endif

void Usart1Init(void);
void Usart1Put(uint8_t c);
uint8_t Usart1Get(void);

#endif /* USART_H_ */
