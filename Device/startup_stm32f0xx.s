/* File: startup_stm32f0xx.s created from startup_ARMCM0.S from CMSIS 3.0 (http://www.onarm.com/CMSIS)
 * Modified for STM32F0XX by https://github.com/viswesr  
 * Date: 20 June 2012
 * ------------------------------------------------------- 
 * File: startup_ARMCM0.S
 * Purpose: startup file for Cortex-M0 devices. Should use with
 *   GCC for ARM Embedded Processors
 * Version: V1.3
 * Date: 08 Feb 2012
 *
 * Copyright (c) 2012, ARM Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the ARM Limited nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ARM LIMITED BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
    .syntax unified
    .arch armv6-m

    .section .stack
    .align 3
#ifdef __STACK_SIZE
    .equ    Stack_Size, __STACK_SIZE
#else
    .equ    Stack_Size, 0x400
#endif
    .globl    __StackTop
    .globl    __StackLimit

    .equ   BootRAM,   0xF108F85F

__StackLimit:
    .space    Stack_Size
    .size __StackLimit, . - __StackLimit
__StackTop:
    .size __StackTop, . - __StackTop

    .section .heap
    .align 3
#ifdef __HEAP_SIZE
    .equ    Heap_Size, __HEAP_SIZE
#else
    .equ    Heap_Size, 0x400
#endif
    .globl    __HeapBase
    .globl    __HeapLimit
__HeapBase:
    .if    Heap_Size
    .space    Heap_Size
    .endif
    .size __HeapBase, . - __HeapBase
__HeapLimit:
    .size __HeapLimit, . - __HeapLimit

    .section .isr_vector
    .align 2
    .globl __isr_vector
__isr_vector:
    .long    __StackTop            /* Top of Stack */
    .long    Reset_Handler         /* Reset Handler */
    .long    NMI_Handler           /* NMI Handler */
    .long    HardFault_Handler     /* Hard Fault Handler */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    SVC_Handler           /* SVCall Handler */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    PendSV_Handler        /* PendSV Handler */
    .long    SysTick_Handler       /* SysTick Handler */

    /* External interrupts */
    .long    SysTick_Handler
    .long    WWDG_IRQHandler
    .long    PVD_IRQHandler
    .long    RTC_IRQHandler
    .long    FLASH_IRQHandler
    .long    RCC_IRQHandler
    .long    EXTI0_1_IRQHandler
    .long    EXTI2_3_IRQHandler
    .long    EXTI4_15_IRQHandler
    .long    TS_IRQHandler
    .long    DMA1_Channel1_IRQHandler
    .long    DMA1_Channel2_3_IRQHandler
    .long    DMA1_Channel4_5_IRQHandler
    .long    ADC1_COMP_IRQHandler 
    .long    TIM1_BRK_UP_TRG_COM_IRQHandler
    .long    TIM1_CC_IRQHandler
    .long    TIM2_IRQHandler
    .long    TIM3_IRQHandler
    .long    TIM6_DAC_IRQHandler
    .long    0
    .long    TIM14_IRQHandler
    .long    TIM15_IRQHandler
    .long    TIM16_IRQHandler
    .long    TIM17_IRQHandler
    .long    I2C1_IRQHandler
    .long    I2C2_IRQHandler
    .long    SPI1_IRQHandler
    .long    SPI2_IRQHandler
    .long    USART1_IRQHandler
    .long    USART2_IRQHandler
    .long    0
    .long    CEC_IRQHandler
    .long    0
    .long    BootRAM

    .size    __isr_vector, . - __isr_vector

    .text
    .thumb
    .thumb_func
    .align 2
    .globl    Reset_Handler
    .type    Reset_Handler, %function
Reset_Handler:
/*     Loop to copy data from read only memory to RAM. The ranges
 *      of copy from/to are specified by following symbols evaluated in
 *      linker script.
 *      __etext: End of code section, i.e., begin of data sections to copy from.
 *      __data_start__/__data_end__: RAM address range that data should be
 *      copied to. Both must be aligned to 4 bytes boundary.  */

    ldr    r1, =__etext
    ldr    r2, =__data_start__
    ldr    r3, =__data_end__

    subs    r3, r2
    ble    .flash_to_ram_loop_end

    movs    r4, 0
.flash_to_ram_loop:
    ldr    r0, [r1,r4]
    str    r0, [r2,r4]
    adds    r4, 4
    cmp    r4, r3
    blt    .flash_to_ram_loop
.flash_to_ram_loop_end:

#ifndef __NO_SYSTEM_INIT
    ldr    r0, =SystemInit
    blx    r0
#endif

    ldr    r0, =_start
    bx    r0
    .pool
    .size Reset_Handler, . - Reset_Handler

/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
    .macro    def_irq_handler    handler_name
    .align 1
    .thumb_func
    .weak    \handler_name
    .type    \handler_name, %function
\handler_name :
    b    .
    .size    \handler_name, . - \handler_name
    .endm

    def_irq_handler    NMI_Handler
    def_irq_handler    HardFault_Handler
    def_irq_handler    SVC_Handler
    def_irq_handler    PendSV_Handler
    def_irq_handler    SysTick_Handler
    def_irq_handler    Default_Handler

    def_irq_handler    WWDG_IRQHandler
    def_irq_handler    PVD_IRQHandler
    def_irq_handler    RTC_IRQHandler
    def_irq_handler    FLASH_IRQHandler
    def_irq_handler    RCC_IRQHandler
    def_irq_handler    EXTI0_1_IRQHandler
    def_irq_handler    EXTI2_3_IRQHandler
    def_irq_handler    EXTI4_15_IRQHandler
    def_irq_handler    TS_IRQHandler
    def_irq_handler    DMA1_Channel1_IRQHandler
    def_irq_handler    DMA1_Channel2_3_IRQHandler
    def_irq_handler    DMA1_Channel4_5_IRQHandler
    def_irq_handler    ADC1_COMP_IRQHandler 
    def_irq_handler    TIM1_BRK_UP_TRG_COM_IRQHandler
    def_irq_handler    TIM1_CC_IRQHandler
    def_irq_handler    TIM2_IRQHandler
    def_irq_handler    TIM3_IRQHandler
    def_irq_handler    TIM6_DAC_IRQHandler
    def_irq_handler    TIM14_IRQHandler
    def_irq_handler    TIM15_IRQHandler
    def_irq_handler    TIM16_IRQHandler
    def_irq_handler    TIM17_IRQHandler
    def_irq_handler    I2C1_IRQHandler
    def_irq_handler    I2C2_IRQHandler
    def_irq_handler    SPI1_IRQHandler
    def_irq_handler    SPI2_IRQHandler
    def_irq_handler    USART1_IRQHandler
    def_irq_handler    USART2_IRQHandler
    def_irq_handler    CEC_IRQHandler

    .end
