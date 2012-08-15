/*
 * buffer.h
 * Circular FIFO
 * http://www.embedds.com/programming-stm32-usart-using-gcc-tools-part-2/
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include "stm32f0xx.h"

#define USARTBUFFSIZE	128

typedef struct {
    uint8_t in;
    uint8_t out;
    uint8_t count;
    uint8_t buff[USARTBUFFSIZE];
} FIFO_TypeDef;

void BufferInit(__IO FIFO_TypeDef *buffer);
ErrorStatus BufferPut(__IO FIFO_TypeDef *buffer, uint8_t c);
ErrorStatus BufferGet(__IO FIFO_TypeDef *buffer, uint8_t *c);
ErrorStatus BufferIsEmpty(__IO FIFO_TypeDef buffer);
ErrorStatus BufferGetLine(__IO FIFO_TypeDef *buffer, char *dst, uint8_t len);

#endif /* BUFFER_H_ */
