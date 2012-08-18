/*
 * usart.c
 */

#include "usart.h"
#include "conf.h"
#include "serial.h"

#ifdef BUFFERED
// initialize buffers
serial_rb rxbuf;
serial_rb txbuf;
SERIAL_RB_Q rx_rb_buf[RXBUFFSIZE];
SERIAL_RB_Q tx_rb_buf[TXBUFFSIZE];
#endif

volatile uint8_t rx_lines_count = 0;
volatile uint8_t tx_lines_count = 0;
uint32_t current_baud = 0;

void usart_1_init(void)
{
#ifdef BUFFERED
    // initialize buffers
    serial_rb_init(&rxbuf, &(rx_rb_buf[0]), RXBUFFSIZE);
    serial_rb_init(&txbuf, &(tx_rb_buf[0]), TXBUFFSIZE);
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
    current_baud = 9600;
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

void usart_1_put(uint8_t c)
{
#ifdef BUFFERED
    // put char to the buffer
    while (serial_rb_full(&txbuf)) {}
    serial_rb_write(&txbuf, (unsigned char)c);
    // enable Transmit Data Register empty interrupt
    USART_ITConfig(EVAL_COM1, USART_IT_TXE, ENABLE);
#else
    USART_SendData(USART1, (uint8_t) c);
    // Loop until the end of transmission
    while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
    {
    }
#endif
}

uint8_t usart_1_get(void)
{
#ifdef BUFFERED
    uint8_t c;
    // check if buffer is empty
    while (serial_rb_empty(&rxbuf)) {}
    c = serial_rb_read(&rxbuf);
    return c;
#else
    while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_RXNE) == RESET);
    return (uint8_t)USART_ReceiveData(EVAL_COM1);
#endif
}

void usart_1_change_baud(uint32_t USART_BaudRate)
{
    /* Disnable USART */
    USART_Cmd(EVAL_COM1, DISABLE);
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = USART_BaudRate;
    current_baud = USART_BaudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(EVAL_COM1, &USART_InitStructure);

    /* Enable USART */
    USART_Cmd(EVAL_COM1, ENABLE);

}

/* libemb stubs */

void serial_init(unsigned int baudrate)
{
    usart_1_init();
}

void serial_send(unsigned char data)
{
    usart_1_put(data);
}

void serial_send_blocking(unsigned char data)
{
    usart_1_put(data);
}

unsigned char serial_recv()
{
    return (unsigned char)usart_1_get();
}

unsigned char serial_recv_blocking()
{
    return (unsigned char)usart_1_get();
}

uint8_t usart_readline(char *dst, uint8_t len)
{
#ifdef BUFFERED
    unsigned char c, idx = 0;
    while (!serial_rb_empty(&rxbuf) && (idx < len - 2))
    {
        c = serial_rb_read(&rxbuf);
        dst[idx++] = c;
        if (c == '\n' || c == '\r')
        {
            dst[--idx] = '\0';
            return idx;
        }
    }
#endif
    return 0;
}
