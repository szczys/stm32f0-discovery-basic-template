/*
 * usart.c
 */

#include "usart.h"

#ifdef BUFFERED
//initialize buffers
volatile FIFO_TypeDef U1Rx, U1Tx;
volatile uint8_t rx_buffer_lines_count = 0;
volatile uint8_t tx_buffer_lines_count = 0;
#endif

void Usart1Init(void)
{
#ifdef BUFFERED
    //initialize buffers
    BufferInit(&U1Rx);
    BufferInit(&U1Tx);
#endif

    /* USARTx configuration ------------------------------------------------------*/
    /* USARTx configured as follow:
      - BaudRate = 9600 baud
      - Word Length = 8 Bits
      - One Stop Bit
      - No parity
      - Hardware flow control disabled (RTS and CTS signals)
      - Receive and transmit enabled
    */
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    STM_EVAL_COMInit(COM1, &USART_InitStructure);

#ifdef BUFFERED

    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure the nested vectored interrupt controller
       Enable the USART1 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = EVAL_COM1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Disable the EVAL_COM1 Transmit interrupt: this interrupt is generated when the
       EVAL_COM1 transmit data register is empty */
    USART_ITConfig(EVAL_COM1, USART_IT_TXE, DISABLE);

    /* Enable the EVAL_COM1 Receive interrupt: this interrupt is generated when the
       EVAL_COM1 receive data register is not empty */
    USART_ITConfig(EVAL_COM1, USART_IT_RXNE, ENABLE);

#endif
}

void Usart1Put(uint8_t c)
{
#ifdef BUFFERED
    //put char to the buffer
    BufferPut(&U1Tx, c);
    //enable Transmit Data Register empty interrupt
    USART_ITConfig(EVAL_COM1, USART_IT_TXE, ENABLE);
#else
    USART_SendData(USART1, (uint8_t) c);
    //Loop until the end of transmission
    while(USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
    {
    }
#endif
}

uint8_t Usart1Get(void){
#ifdef BUFFERED
    uint8_t c;
    //check if buffer is empty
    while (BufferIsEmpty(U1Rx) == SUCCESS);
    BufferGet(&U1Rx, &c);
    return c;
#else
    while ( USART_GetFlagStatus(EVAL_COM1, USART_FLAG_RXNE) == RESET);
    return (uint8_t)USART_ReceiveData(EVAL_COM1);
#endif
}

