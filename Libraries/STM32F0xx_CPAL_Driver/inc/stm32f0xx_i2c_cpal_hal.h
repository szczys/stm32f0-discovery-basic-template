/**
  ******************************************************************************
  * @file    stm32f0xx_i2c_cpal_hal.h
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    24-July-2014
  * @brief   This file contains all the functions prototypes for the CPAL_I2C_HAL
  *          firmware layer.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ___STM32F0XX_I2C_CPAL_HAL_H
#define ___STM32F0XX_I2C_CPAL_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/*========= STM32 Standard library files includes =========*/
#include "stm32f0xx.h"
#include "stm32f0xx_i2c.h"
#include "stm32f0xx_dma.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_misc.h"

/*========= CPAL library files includes =========*/
#include "stm32f0xx_i2c_cpal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/*======================================================================================================================================
                                                 CPAL Hardware Configuration
========================================================================================================================================*/

/*  ------ Configure the communication device and all related peripherals ( GPIO Pin, DMA Channels,
            NVIC Priority) with this file, by referring to configuration Sections:

                - Section 1 : Select the pins to be used for each device instance.

                - Section 2 : Select TX and RX DMA Channels (if DMA mode will be used).

                - Section 3 : Set device's Events, Errors and DMA Interrupts Priorities.                   */


/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/

/*    -- Section 1 :                 **** Device IO Pins Selection ****

  Description: This section allows user to choose IO Pins for each device if possible (in accordance with
               used product: some products have only one possibility for the IO pins).
               Each device instance (I2C1, I2C2 ..) has its specific defines: one for each Pin.
               For each device instance, you will change existing defines with adequate IO Pins and Port
               ( Refer to Product Pin mapping in related datasheet).*/

/* To configure SCL and SDA Pin change these defines with adequate value :

#define CPAL_I2C1_SCL_GPIO_PORT         GPIOX                  (X : Name of the GPIO PORT  (A,B,C,....))
#define CPAL_I2C1_SCL_GPIO_CLK          RCC_APB2Periph_GPIOX   (X : Name of the GPIO PORT  (A,B,C,....))
#define CPAL_I2C1_SCL_GPIO_PIN          GPIO_Pin_X             (X : Pin number (1,2,3,....))
#define CPAL_I2C1_SCL_GPIO_PINSOURCE    GPIO_PinSourceX        (X : Pin number (1,2,3,....))

#define CPAL_I2C1_SDA_GPIO_PORT         GPIOX                  (X : Name of the GPIO PORT  (A,B,C,....))
#define CPAL_I2C1_SDA_GPIO_CLK          RCC_APB2Periph_GPIOX   (X : Name of the GPIO PORT  (A,B,C,....))
#define CPAL_I2C1_SDA_GPIO_PIN          GPIO_Pin_X             (X : Pin number (1,2,3,....))
#define CPAL_I2C1_SDA_GPIO_PINSOURCE    GPIO_PinSourceX        (X : Pin number (1,2,3,....))                   */

/* IO Pins selection possibilities

|--------|---------|--------------|-----------|------------------|-------------------------|
| Device | I2C PIN |   GPIO_PIN   | GPIO_PORT |  GPIO_PinSource  |        GPIO_CLK         |
|--------|---------|--------------|-----------|------------------|-------------------------|
|        |         |  GPIO_Pin_6  |   GPIOB   | GPIO_PinSource6  |   RCC_AHBPeriph_GPIOB   |
|        |   SCL   |--------------|-----------|------------------|-------------------------|
|        |         |  GPIO_Pin_8  |   GPIOB   | GPIO_PinSource8  |   RCC_AHBPeriph_GPIOB   |
|  I2C1  |---------|--------------|-----------|------------------|-------------------------|
|        |         |  GPIO_Pin_7  |   GPIOB   | GPIO_PinSource7  |   RCC_AHBPeriph_GPIOB   |
|        |   SDA   |--------------|-----------|------------------|-------------------------|
|        |         |  GPIO_Pin_9  |   GPIOB   | GPIO_PinSource9  |   RCC_AHBPeriph_GPIOB   |
|--------|---------|--------------|-----------|------------------|-------------------------|
|        |         |  GPIO_Pin_10 |   GPIOB   | GPIO_PinSource10 |   RCC_AHBPeriph_GPIOB   |
|        |   SCL   |--------------|-----------|------------------|-------------------------|
|        |         |  GPIO_Pin_6  |   GPIOF   | GPIO_PinSource6  |   RCC_AHBPeriph_GPIOF   |
|  I2C2  |---------|--------------|-----------|------------------|-------------------------|
|        |         |  GPIO_Pin_11 |   GPIOB   | GPIO_PinSource11 |   RCC_AHBPeriph_GPIOB   |
|        |   SDA   |--------------|-----------|------------------|-------------------------|
|        |         |  GPIO_Pin_7  |   GPIOF   | GPIO_PinSource9  |   RCC_AHBPeriph_GPIOF   |
|--------|---------|--------------|-----------|------------------|-------------------------|

  */


/*----------- I2C1 Device -----------*/

#define CPAL_I2C1_SCL_GPIO_PORT         GPIOB
#define CPAL_I2C1_SCL_GPIO_CLK          RCC_AHBPeriph_GPIOB
#define CPAL_I2C1_SCL_GPIO_PIN          GPIO_Pin_6
#define CPAL_I2C1_SCL_GPIO_PINSOURCE    GPIO_PinSource6

#define CPAL_I2C1_SDA_GPIO_PORT         GPIOB
#define CPAL_I2C1_SDA_GPIO_CLK          RCC_AHBPeriph_GPIOB
#define CPAL_I2C1_SDA_GPIO_PIN          GPIO_Pin_7
#define CPAL_I2C1_SDA_GPIO_PINSOURCE    GPIO_PinSource7

/*-----------I2C2 Device -----------*/

#define CPAL_I2C2_SCL_GPIO_PORT         GPIOB
#define CPAL_I2C2_SCL_GPIO_CLK          RCC_AHBPeriph_GPIOB
#define CPAL_I2C2_SCL_GPIO_PIN          GPIO_Pin_10
#define CPAL_I2C2_SCL_GPIO_PINSOURCE    GPIO_PinSource10

#define CPAL_I2C2_SDA_GPIO_PORT         GPIOB
#define CPAL_I2C2_SDA_GPIO_CLK          RCC_AHBPeriph_GPIOB
#define CPAL_I2C2_SDA_GPIO_PIN          GPIO_Pin_11
#define CPAL_I2C2_SDA_GPIO_PINSOURCE    GPIO_PinSource11

/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/

/*    -- Section 2 :           **** Device TX and RX DMA Channels Selection ****

  Description: This section allows user to choose TX and RX DMA Channels if possible (in accordance with
               used product) for each device.
               Each device instance (I2C1, I2C2 ..) has its specific defines: one for DMA TX Channel and
               another one for DMA RX Channel.
               For each device instance, you find all TX an RX DMA Channel possibilities ( Refer to Product
               Reference Manual).*/

/* DMA Channel selection possibilities

|--------|---------|----------------|
| Device | Channel |  DMA Channel   |
|--------|---------|----------------|
|        |    TX   | DMA1_Channel2  |
|  I2C1  |---------|----------------|
|        |    RX   | DMA1_Channel3  |
|--------|---------|----------------|
|        |    TX   | DMA1_Channel4  |
|  I2C2  |---------|----------------|
|        |    RX   | DMA1_Channel5  |
|--------|---------|----------------|*/

/* I2Cx TX and RX DMA channels for STM32F0XX family are fixed */

/*----------- I2C1 Device -----------*/
#define CPAL_I2C1_DMA_TX_Channel        DMA1_Channel2
#define CPAL_I2C1_DMA_RX_Channel        DMA1_Channel3

/*----------- I2C2 Device -----------*/
#define CPAL_I2C2_DMA_TX_Channel        DMA1_Channel4
#define CPAL_I2C2_DMA_RX_Channel        DMA1_Channel5

/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/

/*   -- Section 3 :             **** I2C and DMA Interrupts Priority Selection ****

  Description: This section allows user to select Interrupt Priority of I2C Event Interrupts and DMA Interrupts.
                 
               Make sure that the following rules are always respected:
                   - I2C event and error interrupts should be interruptible (mainly, the timeout interrupt should
                     be able to interrupt all device ISR)
                   - I2C Error interrupt priority should be higher than Event interrupt
                   - The timeout mechanism interrupt priority should be the highest one and it should be able to
                     interrupt any other ISR.
                   - It is advised that DMA interrupts have higher priority than the device event interrupts.*/


/*----------- I2Cx Interrupt Priority -------------*/

/*----------- I2C1 Device -----------*/
#define I2C1_IT_PRIO                 I2C1_IT_OFFSET_PREPRIO + 2   /* I2C1 IT PRIORITY */
#define I2C1_IT_DMA_PRIO             I2C1_IT_OFFSET_PREPRIO + 0   /* I2C1 DMA PRIORITY */

/*----------- I2C2 Device -----------*/
#define I2C2_IT_PRIO                 I2C2_IT_OFFSET_PREPRIO + 2   /* I2C2 IT PRIORITY */
#define I2C2_IT_DMA_PRIO             I2C2_IT_OFFSET_PREPRIO + 0   /* I2C2 DMA PRIORITY */


/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
 
/*****END OF CPAL Hardware Configuration***************************************************************************************************/

  /* !WARNING!:
     --------- 
     The following code should not be modified by user.
     Any modification may cause Library dysfunction.
  */

/*========= Common Defines =========*/  
  
/* This define set the number of I2C devices that can be used with this product family */
#define CPAL_I2C_DEV_NUM     2   

/* This define is used to enable DMA Channel */
#define CPAL_DMA_CCR_EN                 DMA_CCR_EN

/* This define is used to check if DMA interrupt option are enabled */
#define CPAL_OPT_DMA_IT_MASK            ((uint32_t)0x00003F00)

/* This define is used to check I2C errors (BERR, ARLO and OVR) */
#define CPAL_I2C_STATUS_ERR_MASK        ((uint32_t)0x00000700)  /*!< I2C errors Mask  */

/* This define is used to check I2C events (TXIS, RXNE, ADDR, NACKF, STOPF, TC and TCR) */
#define CPAL_I2C_STATUS_EVT_MASK       ((uint16_t)0x0000000FE)  /*!< I2C event Mask for Status Register 1 */

/* This define is used to check if DMA TX interrupt are selected */
#define CPAL_OPT_I2C_DMA_TX_IT_MASK     ((uint32_t)0x00000700)

/* This define is used to check if DMA RX interrupt are selected */
#define CPAL_OPT_I2C_DMA_RX_IT_MASK     ((uint32_t)0x00003800)

 /* I2C Event Defines */
#define CPAL_I2C_EVT_ADDR               I2C_ISR_ADDR     /*!<Address matched */
#define CPAL_I2C_EVT_STOP               I2C_ISR_STOPF    /*!<Stop detection */
#define CPAL_I2C_EVT_NACK               I2C_ISR_NACKF    /*!<NACK received */
#define CPAL_I2C_EVT_RXNE               I2C_ISR_RXNE     /*!<Receive Data Register not Empty */
#define CPAL_I2C_EVT_TXIS               I2C_ISR_TXIS     /*!<Transmit interrupt Status */
#define CPAL_I2C_EVT_TCR                I2C_ISR_TCR      /*!<Transfer Complete Reload */
#define CPAL_I2C_EVT_TC                 I2C_ISR_TC       /*!<Transfer Complete */

/*========= I2C1 specific defines (GPIO, PINs, Clocks and DMA) =========*/   

#define CPAL_I2C1_CLK                   RCC_APB1Periph_I2C1
#define CPAL_I2C1_TXDR                  ((uint32_t)0x40005428)
#define CPAL_I2C1_RXDR                  ((uint32_t)0x40005424)
#define CPAL_I2C1_AF                    GPIO_AF_1

#define CPAL_I2C1_DMA                   DMA1
#define CPAL_I2C1_DMA_CLK               RCC_AHBPeriph_DMA1

#define CPAL_I2C1_IT_IRQn               I2C1_IRQn
#define CPAL_I2C1_DMA_IRQn              DMA1_Channel2_3_IRQn

#define CPAL_I2C1_DMA_IRQHandler        DMA1_Channel2_3_IRQHandler

#define CPAL_I2C1_DMA_TX_TC_FLAG        DMA1_FLAG_TC2
#define CPAL_I2C1_DMA_TX_HT_FLAG        DMA1_FLAG_HT2
#define CPAL_I2C1_DMA_TX_TE_FLAG        DMA1_FLAG_TE2

#define CPAL_I2C1_DMA_RX_TC_FLAG        DMA1_FLAG_TC3
#define CPAL_I2C1_DMA_RX_HT_FLAG        DMA1_FLAG_HT3
#define CPAL_I2C1_DMA_RX_TE_FLAG        DMA1_FLAG_TE3

/*========= I2C2 specific defines (GPIO, PINs, Clocks and DMA) =========*/

#define CPAL_I2C2_CLK                   RCC_APB1Periph_I2C2
#define CPAL_I2C2_TXDR                  ((uint32_t)0x40005828)
#define CPAL_I2C2_RXDR                  ((uint32_t)0x40005824)
#define CPAL_I2C2_AF                    GPIO_AF_1

#define CPAL_I2C2_DMA                   DMA1
#define CPAL_I2C2_DMA_CLK               RCC_AHBPeriph_DMA1

#define CPAL_I2C2_IT_IRQn               I2C2_IRQn
#ifdef STM32F072
 #define CPAL_I2C2_DMA_IRQn              DMA1_Channel4_5_6_7_IRQn
 #define CPAL_I2C2_DMA_IRQHandler        DMA1_Channel4_5_6_7IRQHandler
#else
 #define CPAL_I2C2_DMA_IRQn              DMA1_Channel4_5_IRQn
 #define CPAL_I2C2_DMA_IRQHandler        DMA1_Channel4_5_IRQHandler
#endif /* STM32F072 */ 



#define CPAL_I2C2_DMA_TX_TC_FLAG        DMA1_FLAG_TC4
#define CPAL_I2C2_DMA_TX_HT_FLAG        DMA1_FLAG_HT4
#define CPAL_I2C2_DMA_TX_TE_FLAG        DMA1_FLAG_TE4

#define CPAL_I2C2_DMA_RX_TC_FLAG        DMA1_FLAG_TC5
#define CPAL_I2C2_DMA_RX_HT_FLAG        DMA1_FLAG_HT5
#define CPAL_I2C2_DMA_RX_TE_FLAG        DMA1_FLAG_TE5


/* Exported macro ------------------------------------------------------------*/

/*========= Peripheral Clock Command =========*/

#define __I2C_CLK_CMD(clk,cmd)                          RCC_APB1PeriphClockCmd((clk),(cmd))

#define __I2C_RCC_RESET(clk)                            RCC_APB1PeriphResetCmd((clk),ENABLE);\
                                                        RCC_APB1PeriphResetCmd((clk),DISABLE)

#define __I2C_GPIO_CLK_CMD(clk,cmd)                     RCC_AHBPeriphClockCmd((clk),(cmd))

#define __DMA_CLK_CMD(clk,cmd)                          RCC_AHBPeriphClockCmd((clk),(cmd))

#define __DMA_RESET_CMD(clk,cmd)                        RCC_AHBPeriphResetCmd((clk),(cmd))


/*========= DMA =========*/
  
/* DMA channels enable/disable */

#define __CPAL_I2C_HAL_ENABLE_DMATX(device)             CPAL_I2C_DMA_TX_Channel[(device)]->CCR |= CPAL_DMA_CCR_EN

#define __CPAL_I2C_HAL_DISABLE_DMATX(device)            CPAL_I2C_DMA_TX_Channel[(device)]->CCR &= ~CPAL_DMA_CCR_EN

#define __CPAL_I2C_HAL_ENABLE_DMARX(device)             CPAL_I2C_DMA_RX_Channel[(device)]->CCR |= CPAL_DMA_CCR_EN

#define __CPAL_I2C_HAL_DISABLE_DMARX(device)            CPAL_I2C_DMA_RX_Channel[(device)]->CCR &= ~CPAL_DMA_CCR_EN

/* DMA interrupts enable/disable */

#define __I2C_HAL_ENABLE_DMATX_TCIT(device)             CPAL_I2C_DMA_TX_Channel[(device)]->CCR |= DMA_IT_TC

#define __I2C_HAL_ENABLE_DMATX_HTIT(device)             CPAL_I2C_DMA_TX_Channel[(device)]->CCR |= DMA_IT_HT

#define __I2C_HAL_ENABLE_DMATX_TEIT(device)             CPAL_I2C_DMA_TX_Channel[(device)]->CCR |= DMA_IT_TE

#define __I2C_HAL_ENABLE_DMARX_TCIT(device)             CPAL_I2C_DMA_RX_Channel[(device)]->CCR |= DMA_IT_TC

#define __I2C_HAL_ENABLE_DMARX_HTIT(device)             CPAL_I2C_DMA_RX_Channel[(device)]->CCR |= DMA_IT_HT

#define __I2C_HAL_ENABLE_DMARX_TEIT(device)             CPAL_I2C_DMA_RX_Channel[(device)]->CCR |= DMA_IT_TE

/* DMA interrupts flag management */
  
#define __CPAL_I2C_HAL_GET_DMATX_IT(device)             (uint32_t)(CPAL_I2C_DMA[(device)]->ISR & (CPAL_I2C_DMA_TX_TC_FLAG[(device)] \
                                                        | CPAL_I2C_DMA_TX_HT_FLAG[(device)] | CPAL_I2C_DMA_TX_TE_FLAG[(device)]))
  
#define __CPAL_I2C_HAL_GET_DMATX_TCIT(device)           (uint32_t)(CPAL_I2C_DMA[(device)]->ISR & CPAL_I2C_DMA_TX_TC_FLAG [(device)])

#define __CPAL_I2C_HAL_GET_DMATX_HTIT(device)           (uint32_t)(CPAL_I2C_DMA[(device)]->ISR & CPAL_I2C_DMA_TX_HT_FLAG [(device)])

#define __CPAL_I2C_HAL_GET_DMATX_TEIT(device)           (uint32_t)(CPAL_I2C_DMA[(device)]->ISR & CPAL_I2C_DMA_TX_TE_FLAG [(device)])
  
#define __CPAL_I2C_HAL_GET_DMARX_IT(device)             (uint32_t)(CPAL_I2C_DMA[(device)]->ISR & (CPAL_I2C_DMA_RX_TC_FLAG[(device)] \
                                                        | CPAL_I2C_DMA_RX_HT_FLAG[(device)] | CPAL_I2C_DMA_RX_TE_FLAG[(device)]))  
  
#define __CPAL_I2C_HAL_GET_DMARX_TCIT(device)           (uint32_t)(CPAL_I2C_DMA[(device)]->ISR & CPAL_I2C_DMA_RX_TC_FLAG [(device)])

#define __CPAL_I2C_HAL_GET_DMARX_HTIT(device)           (uint32_t)(CPAL_I2C_DMA[(device)]->ISR & CPAL_I2C_DMA_RX_HT_FLAG [(device)])

#define __CPAL_I2C_HAL_GET_DMARX_TEIT(device)           (uint32_t)(CPAL_I2C_DMA[(device)]->ISR & CPAL_I2C_DMA_RX_TE_FLAG [(device)])

#define __CPAL_I2C_HAL_CLEAR_DMATX_IT(device)           CPAL_I2C_DMA[(device)]->IFCR = (CPAL_I2C_DMA_TX_TC_FLAG[(device)] \
                                                        | CPAL_I2C_DMA_TX_HT_FLAG[(device)] | CPAL_I2C_DMA_TX_TE_FLAG[(device)])

#define __CPAL_I2C_HAL_CLEAR_DMARX_IT(device)           CPAL_I2C_DMA[(device)]->IFCR = (CPAL_I2C_DMA_RX_TC_FLAG[(device)] \
                                                        | CPAL_I2C_DMA_RX_HT_FLAG[(device)] | CPAL_I2C_DMA_RX_TE_FLAG[(device)])

/* Get DMA data counter */

#define __CPAL_I2C_HAL_DMATX_GET_CNDT(device)           (uint32_t)(CPAL_I2C_DMA_TX_Channel[(device)]->CNDTR)

#define __CPAL_I2C_HAL_DMARX_GET_CNDT(device)           (uint32_t)(CPAL_I2C_DMA_RX_Channel[(device)]->CNDTR)


/*========= I2C =========*/

/* I2C enable/disable */

#define __CPAL_I2C_HAL_ENABLE_DEV(device)               CPAL_I2C_DEVICE[(device)]->CR1 |= I2C_CR1_PE

#define __CPAL_I2C_HAL_DISABLE_DEV(device)              CPAL_I2C_DEVICE[(device)]->CR1 &= ~I2C_CR1_PE

/* I2C software reset */

#define __CPAL_I2C_HAL_SWRST(device)                    CPAL_I2C_DEVICE[(device)]->CR1 |= I2C_CR1_SWRST; \
                                                        CPAL_I2C_DEVICE[(device)]->CR1 &= ~I2C_CR1_SWRST

/* I2C Wakeup option */

#define __CPAL_I2C_HAL_ENABLE_WAKEUP(device)               CPAL_I2C_DEVICE[(device)]->CR1 |= I2C_CR1_WUPEN

#define __CPAL_I2C_HAL_DISABLE_WAKEUP(device)              CPAL_I2C_DEVICE[(device)]->CR1 &= ~I2C_CR1_WUPEN
  
/* I2C interrupts enable/disable */
  
#define __CPAL_I2C_HAL_DISABLE_ALLIT(device)            CPAL_I2C_DEVICE[(device)]->CR1 &= ~(I2C_CR1_TXIE | I2C_CR1_RXIE | I2C_CR1_ADDRIE | \
                                                        I2C_CR1_STOPIE | I2C_CR1_TCIE | I2C_CR1_ERRIE | I2C_CR1_NACKIE)
  
#define __CPAL_I2C_HAL_ENABLE_ERRIT(device)             CPAL_I2C_DEVICE[(device)]->CR1 |= (I2C_CR1_ERRIE | I2C_CR1_NACKIE)

#define __CPAL_I2C_HAL_DISABLE_ERRIT(device)            CPAL_I2C_DEVICE[(device)]->CR1 &= ~(I2C_CR1_ERRIE | I2C_CR1_NACKIE)

  
#define __CPAL_I2C_HAL_ENABLE_MASTER_IT(device)         CPAL_I2C_DEVICE[(device)]->CR1 |= (I2C_CR1_TCIE | I2C_CR1_STOPIE)

#define __CPAL_I2C_HAL_DISABLE_MASTER_IT(device)        CPAL_I2C_DEVICE[(device)]->CR1 &= ~(I2C_CR1_TCIE | I2C_CR1_STOPIE)
  
#define __CPAL_I2C_HAL_ENABLE_MASTER_TXIT(device)       CPAL_I2C_DEVICE[(device)]->CR1 |= (I2C_CR1_TCIE | I2C_CR1_STOPIE | I2C_CR1_TXIE)

#define __CPAL_I2C_HAL_ENABLE_MASTER_RXIT(device)       CPAL_I2C_DEVICE[(device)]->CR1 |= (I2C_CR1_TCIE | I2C_CR1_STOPIE | I2C_CR1_RXIE)
  
  
#define __CPAL_I2C_HAL_ENABLE_SLAVE_IT(device)          CPAL_I2C_DEVICE[(device)]->CR1 |= (I2C_CR1_ADDRIE | I2C_CR1_STOPIE)

#define __CPAL_I2C_HAL_DISABLE_SLAVE_IT(device)         CPAL_I2C_DEVICE[(device)]->CR1 &= ~(I2C_CR1_ADDRIE | I2C_CR1_STOPIE)

#define __CPAL_I2C_HAL_ENABLE_SLAVE_TXIT(device)        CPAL_I2C_DEVICE[(device)]->CR1 |= (I2C_CR1_ADDRIE | I2C_CR1_STOPIE | I2C_CR1_TXIE)

#define __CPAL_I2C_HAL_ENABLE_SLAVE_RXIT(device)        CPAL_I2C_DEVICE[(device)]->CR1 |= (I2C_CR1_ADDRIE | I2C_CR1_STOPIE | I2C_CR1_RXIE)
  
  
#define __CPAL_I2C_HAL_ENABLE_STOPIE_IT(device)         CPAL_I2C_DEVICE[(device)]->CR1 |= I2C_CR1_STOPIE

#define __CPAL_I2C_HAL_DISABLE_STOPIE_IT(device)        CPAL_I2C_DEVICE[(device)]->CR1 &= ~I2C_CR1_STOPIE

#define __CPAL_I2C_HAL_ENABLE_ADDRIE_IT(device)         CPAL_I2C_DEVICE[(device)]->CR1 |= I2C_CR1_ADDRIE

#define __CPAL_I2C_HAL_DISABLE_ADDRIE_IT(device)        CPAL_I2C_DEVICE[(device)]->CR1 &= ~I2C_CR1_ADDRIE
 
#define __CPAL_I2C_HAL_ENABLE_TCIE_IT(device)           CPAL_I2C_DEVICE[(device)]->CR1 |= I2C_CR1_TCIE

#define __CPAL_I2C_HAL_DISABLE_TCIE_IT(device)          CPAL_I2C_DEVICE[(device)]->CR1 &= ~I2C_CR1_TCIE
  
#define __CPAL_I2C_HAL_ENABLE_TXIE_IT(device)           CPAL_I2C_DEVICE[(device)]->CR1 |= I2C_CR1_TXIE

#define __CPAL_I2C_HAL_DISABLE_TXIE_IT(device)          CPAL_I2C_DEVICE[(device)]->CR1 &= ~I2C_CR1_TXIE

#define __CPAL_I2C_HAL_ENABLE_RXIE_IT(device)           CPAL_I2C_DEVICE[(device)]->CR1 |= I2C_CR1_RXIE

#define __CPAL_I2C_HAL_DISABLE_RXIE_IT(device)          CPAL_I2C_DEVICE[(device)]->CR1 &= ~I2C_CR1_RXIE
    

/* I2C Addressing configuration */

#define __CPAL_I2C_HAL_SADD_CONF(device,value)          CPAL_I2C_DEVICE[(device)]->CR2 &= ~I2C_CR2_SADD; \
                                                        CPAL_I2C_DEVICE[(device)]->CR2 |= (uint32_t)((value) & 0x000003FF)

#define __CPAL_I2C_HAL_OA2_CONF(device,value)           CPAL_I2C_DEVICE[(device)]->OAR2 &= ~I2C_OAR2_OA2; \
                                                        CPAL_I2C_DEVICE[(device)]->OAR2 |= (uint32_t)((value) & 0x000000FE)

#define __CPAL_I2C_HAL_OA2_MASK_CONF(device,value)      CPAL_I2C_DEVICE[(device)]->OAR2 &= ~I2C_OAR2_OA2MSK; \
                                                        CPAL_I2C_DEVICE[(device)]->OAR2 |= (uint32_t)((value) << 8)

#define __CPAL_I2C_HAL_ENABLE_OA2(device)               CPAL_I2C_DEVICE[(device)]->OAR2 |= I2C_OAR2_OA2EN

#define __CPAL_I2C_HAL_ENABLE_ADD10(device)             CPAL_I2C_DEVICE[(device)]->CR2 |= I2C_CR2_ADD10

#define __CPAL_I2C_HAL_DISABLE_ADD10(device)            CPAL_I2C_DEVICE[(device)]->CR2 &= ~I2C_CR2_ADD10

#define __CPAL_I2C_HAL_ENABLE_COMPLETE_HEAD10R(device)  CPAL_I2C_DEVICE[(device)]->CR2 &= ~I2C_CR2_HEAD10R

#define __CPAL_I2C_HAL_DISABLE_COMPLETE_HEAD10R(device) CPAL_I2C_DEVICE[(device)]->CR2 |= I2C_CR2_HEAD10R

#define __CPAL_I2C_HAL_ENABLE_GENCALL(device)           CPAL_I2C_DEVICE[(device)]->CR1 |= I2C_CR1_GCEN

#define __CPAL_I2C_HAL_REQ_WRITE_TRANSFER(device)       CPAL_I2C_DEVICE[(device)]->CR2 &= ~I2C_CR2_RD_WRN

#define __CPAL_I2C_HAL_REQ_READ_TRANSFER(device)        CPAL_I2C_DEVICE[(device)]->CR2 |= I2C_CR2_RD_WRN

#define __CPAL_I2C_HAL_GET_OA1(device)                  (uint32_t)(CPAL_I2C_DEVICE[(device)]->ISR & I2C_OAR1_OA1)

#define __CPAL_I2C_HAL_GET_OA2(device)                  (uint32_t)(CPAL_I2C_DEVICE[(device)]->ISR & I2C_OAR2_OA2)
  
#define __CPAL_I2C_HAL_GET_OA2_MASK(device)             (uint32_t)((CPAL_I2C_DEVICE[(device)]->ISR & I2C_OAR2_OA2MSK) >> 8)

#define __CPAL_I2C_HAL_GET_ADDCODE(device)              (uint32_t)((CPAL_I2C_DEVICE[(device)]->ISR & I2C_ISR_ADDCODE) >> 17)
  
#define __CPAL_I2C_HAL_GET_DIR(device)                  (uint32_t)(CPAL_I2C_DEVICE[(device)]->ISR & I2C_ISR_DIR)

/* I2C misc configuration */
  
#define __CPAL_I2C_HAL_CR2_UPDATE(device,value)         CPAL_I2C_DEVICE[(device)]->CR2 = value
  
#define __CPAL_I2C_HAL_ENABLE_TXDMAREQ(device)          CPAL_I2C_DEVICE[(device)]->CR1 |= I2C_CR1_TXDMAEN

#define __CPAL_I2C_HAL_DISABLE_TXDMAREQ(device)         CPAL_I2C_DEVICE[(device)]->CR1 &= ~I2C_CR1_TXDMAEN

#define __CPAL_I2C_HAL_ENABLE_RXDMAREQ(device)          CPAL_I2C_DEVICE[(device)]->CR1 |= I2C_CR1_RXDMAEN

#define __CPAL_I2C_HAL_DISABLE_RXDMAREQ(device)         CPAL_I2C_DEVICE[(device)]->CR1 &= ~I2C_CR1_RXDMAEN

#define __CPAL_I2C_HAL_ENABLE_NACK(device)              CPAL_I2C_DEVICE[(device)]->CR2 |= I2C_CR2_NACK

#define __CPAL_I2C_HAL_DISABLE_NACK(device)             CPAL_I2C_DEVICE[(device)]->CR2 &= ~I2C_CR2_NACK

#define __CPAL_I2C_HAL_ENABLE_AUTOEND(device)           CPAL_I2C_DEVICE[(device)]->CR2 |= I2C_CR2_AUTOEND

#define __CPAL_I2C_HAL_DISABLE_AUTOEND(device)          CPAL_I2C_DEVICE[(device)]->CR2 &= ~I2C_CR2_AUTOEND

#define __CPAL_I2C_HAL_ENABLE_RELOAD(device)            CPAL_I2C_DEVICE[(device)]->CR2 |= I2C_CR2_RELOAD

#define __CPAL_I2C_HAL_DISABLE_RELOAD(device)           CPAL_I2C_DEVICE[(device)]->CR2 &= ~I2C_CR2_RELOAD

#define __CPAL_I2C_HAL_ENABLE_NOSTRETCH(device)         CPAL_I2C_DEVICE[(device)]->CR1 |= I2C_CR1_NOSTRETCH

#define __CPAL_I2C_HAL_DISABLE_NOSTRETCH(device)        CPAL_I2C_DEVICE[(device)]->CR1 &= ~I2C_CR1_NOSTRETCH

#define __CPAL_I2C_HAL_START(device)                    CPAL_I2C_DEVICE[(device)]->CR2 |= I2C_CR2_START

#define __CPAL_I2C_HAL_STOP(device)                     CPAL_I2C_DEVICE[(device)]->CR2 |= I2C_CR2_STOP

/* I2C data management */

#define __CPAL_I2C_HAL_RECEIVE(device)                  (uint8_t)(CPAL_I2C_DEVICE[(device)]->RXDR)

#define __CPAL_I2C_HAL_SEND(device,value)               CPAL_I2C_DEVICE[(device)]->TXDR = (uint8_t)((value))

#define __CPAL_I2C_HAL_SET_NBYTES(device,value)         CPAL_I2C_DEVICE[(device)]->CR2 &= ~I2C_CR2_NBYTES; \
                                                        CPAL_I2C_DEVICE[(device)]->CR2 |= (uint32_t)((uint32_t)(value) << 16)
  
#define __CPAL_I2C_HAL_GET_NBYTES(device,value)         (uint32_t)((CPAL_I2C_DEVICE[(device)]->CR2 & I2C_CR2_NBYTES) >> 16)

#define __CPAL_I2C_HAL_CLEAR_NBYTES(device)             CPAL_I2C_DEVICE[(device)]->CR2 &= ~I2C_CR2_NBYTES  

/* I2C flags management */

#define __CPAL_I2C_HAL_GET_EVENT(device)                (uint32_t)(CPAL_I2C_DEVICE[(device)]->ISR & CPAL_I2C_STATUS_EVT_MASK)

#define __CPAL_I2C_HAL_GET_ERROR(device)                (uint32_t)(CPAL_I2C_DEVICE[(device)]->ISR & CPAL_I2C_STATUS_ERR_MASK)

#define __CPAL_I2C_HAL_GET_TXE(device)                  (uint32_t)(CPAL_I2C_DEVICE[(device)]->ISR & I2C_ISR_TXE)

#define __CPAL_I2C_HAL_GET_TXIS(device)                 (uint32_t)(CPAL_I2C_DEVICE[(device)]->ISR & I2C_ISR_TXIS)

#define __CPAL_I2C_HAL_GET_RXNE(device)                 (uint32_t)(CPAL_I2C_DEVICE[(device)]->ISR & I2C_ISR_RXNE)

#define __CPAL_I2C_HAL_GET_ADDR(device)                 (uint32_t)(CPAL_I2C_DEVICE[(device)]->ISR & I2C_ISR_ADDR)

#define __CPAL_I2C_HAL_GET_NACK(device)                 (uint32_t)(CPAL_I2C_DEVICE[(device)]->ISR & I2C_ISR_NACKF)

#define __CPAL_I2C_HAL_GET_STOP(device)                 (uint32_t)(CPAL_I2C_DEVICE[(device)]->ISR & I2C_ISR_STOPF)

#define __CPAL_I2C_HAL_GET_TC(device)                   (uint32_t)(CPAL_I2C_DEVICE[(device)]->ISR & I2C_ISR_TC)

#define __CPAL_I2C_HAL_GET_TCR(device)                  (uint32_t)(CPAL_I2C_DEVICE[(device)]->ISR & I2C_ISR_TCR)

#define __CPAL_I2C_HAL_GET_BERR(device)                 (uint32_t)(CPAL_I2C_DEVICE[(device)]->ISR & I2C_ISR_BERR)

#define __CPAL_I2C_HAL_GET_ARLO(device)                 (uint32_t)(CPAL_I2C_DEVICE[(device)]->ISR & I2C_ISR_ARLO)

#define __CPAL_I2C_HAL_GET_OVR(device)                  (uint32_t)(CPAL_I2C_DEVICE[(device)]->ISR & I2C_ISR_OVR)

#define __CPAL_I2C_HAL_GET_BUSY(device)                 (uint32_t)(CPAL_I2C_DEVICE[(device)]->ISR & I2C_ISR_BUSY)

#define __CPAL_I2C_HAL_CLEAR_ADDR(device)               CPAL_I2C_DEVICE[(device)]->ICR = I2C_ICR_ADDRCF

#define __CPAL_I2C_HAL_CLEAR_NACK(device)               CPAL_I2C_DEVICE[(device)]->ICR = I2C_ICR_NACKCF

#define __CPAL_I2C_HAL_CLEAR_STOP(device)               CPAL_I2C_DEVICE[(device)]->ICR = I2C_ICR_STOPCF

#define __CPAL_I2C_HAL_CLEAR_BERR(device)               CPAL_I2C_DEVICE[(device)]->ICR = I2C_ICR_BERRCF

#define __CPAL_I2C_HAL_CLEAR_ARLO(device)               CPAL_I2C_DEVICE[(device)]->ICR = I2C_ICR_ARLOCF

#define __CPAL_I2C_HAL_CLEAR_OVR(device)                CPAL_I2C_DEVICE[(device)]->ICR = I2C_ICR_OVRCF
  
/* Exported functions --------------------------------------------------------*/

/*========= I2CX IRQHandler =========*/

#ifdef CPAL_USE_I2C1
   uint32_t I2C1_IRQHandler(void); /*<!I2C1 Interrupt Handler : handle Communication and errors of I2C1 Device */

#endif /* CPAL_USE_I2C1 */

#ifdef CPAL_USE_I2C2
   uint32_t I2C2_IRQHandler(void); /*<!I2C1 Interrupt Handler : handle Communication and errors of I2C2 Device */
#endif /* CPAL_USE_I2C2 */


#ifdef CPAL_I2C_DMA_PROGMODEL

/*========= DMA I2CX IRQHandler =========*/

#ifdef CPAL_USE_I2C1
   uint32_t CPAL_I2C1_DMA_IRQHandler(void); /*<!I2C1 DMA Interrupt Handler : handle data Transmission and reception with DMA */
#endif /* CPAL_USE_I2C1 */

#ifdef CPAL_USE_I2C2
   uint32_t CPAL_I2C2_DMA_IRQHandler(void); /*<!I2C2 DMA Interrupt Handler : handle data Transmission and reception with DMA */
#endif /* CPAL_USE_I2C2 */

#endif /* CPAL_I2C_DMA_PROGMODEL */ 

/*========= Hardware Abstraction Layer local =========*/

  void CPAL_I2C_HAL_CLKInit(CPAL_DevTypeDef Device);    /*<!This function resets then enable the I2C device clock */

  void CPAL_I2C_HAL_CLKDeInit(CPAL_DevTypeDef Device);  /*<!This function resets then disable the I2C device clock */

  void CPAL_I2C_HAL_GPIOInit(CPAL_DevTypeDef Device);   /*<!This function configures the IO pins used by the I2C device */

  void CPAL_I2C_HAL_GPIODeInit(CPAL_DevTypeDef Device); /*<!This function deinitialize the IO pins used by the I2C device
                                                            (configured to their default state) */

#ifdef CPAL_I2C_DMA_PROGMODEL
  void CPAL_I2C_HAL_DMAInit(CPAL_DevTypeDef Device, CPAL_DirectionTypeDef Direction, uint32_t Options); /*<!This function enable the DMA clock and initialize
                                                                                                            needed DMA Channels used by the I2C device*/

  void CPAL_I2C_HAL_DMATXConfig(CPAL_DevTypeDef Device,CPAL_TransferTypeDef* TransParameter, uint32_t Options); /*<!This function configures the DMA channel specific
                                                                                                                    for TX transfer */

  void CPAL_I2C_HAL_DMARXConfig(CPAL_DevTypeDef Device,CPAL_TransferTypeDef* TransParameter, uint32_t Options); /*<!This function configures the DMA channel specific
                                                                                                                    for RX transfer */

  void CPAL_I2C_HAL_DMADeInit(CPAL_DevTypeDef Device, CPAL_DirectionTypeDef Direction); /*<!This function deinitialize the DMA channel used
                                                                                            by I2C Device (Configure them to their default
                                                                                            values). DMA clock is not disabled */
#endif /* CPAL_I2C_DMA_PROGMODEL */

  void CPAL_I2C_HAL_ITInit(CPAL_DevTypeDef Device, uint32_t Options); /*<!This function configures NVIC and interrupts used
                                                                          by I2C Device according to enabled options */

  void CPAL_I2C_HAL_ITDeInit(CPAL_DevTypeDef Device, uint32_t Options); /*<!This function deinitialize NVIC and interrupts used
                                                                          by I2C Device  */


#ifdef __cplusplus
}
#endif

#endif /*___STM32F0XX_I2C_CPAL_HAL_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
