/**
  ******************************************************************************
  * @file    stm32f0_discovery.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   This file contains definitions for STM32F0-Discovery's Leds, push-
  *          buttons hardware resources.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
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
#ifndef __STM32F0_DISCOVERY_H
#define __STM32F0_DISCOVERY_H

#ifdef __cplusplus
extern "C" {
#endif

    /* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

    /** @addtogroup Utilities
      * @{
      */

    /** @addtogroup STM32F0_DISCOVERY
      * @{
      */

    /** @addtogroup STM32F0_DISCOVERY_LOW_LEVEL
      * @{
      */

    /** @defgroup STM32F0_DISCOVERY_LOW_LEVEL_Exported_Types
      * @{
      */
    typedef enum
    {
        LED3 = 0,
        LED4 = 1
    } Led_TypeDef;

    typedef enum
    {
        BUTTON_USER = 0,
    } Button_TypeDef;

    typedef enum
    {
        BUTTON_MODE_GPIO = 0,
        BUTTON_MODE_EXTI = 1
    } ButtonMode_TypeDef;

    typedef enum
    {
        COM1 = 0,
        COM2 = 1
    } COM_TypeDef;

    /**
      * @}
      */

    /** @defgroup STM32F0_DISCOVERY_LOW_LEVEL_Exported_Constants
      * @{
      */

    /** @addtogroup STM32F0_DISCOVERY_LOW_LEVEL_LED
      * @{
      */
#define LEDn                             2

#define LED3_PIN                         GPIO_Pin_9
#define LED3_GPIO_PORT                   GPIOC
#define LED3_GPIO_CLK                    RCC_AHBPeriph_GPIOC

#define LED4_PIN                         GPIO_Pin_8
#define LED4_GPIO_PORT                   GPIOC
#define LED4_GPIO_CLK                    RCC_AHBPeriph_GPIOC

    /**
      * @}
      */

    /** @addtogroup SSTM32F0_DISCOVERY_LOW_LEVEL_BUTTON
      * @{
      */
#define BUTTONn                          1

    /**
     * @brief USER push-button
     */
#define USER_BUTTON_PIN                GPIO_Pin_0
#define USER_BUTTON_GPIO_PORT          GPIOA
#define USER_BUTTON_GPIO_CLK           RCC_AHBPeriph_GPIOA
#define USER_BUTTON_EXTI_LINE          EXTI_Line0
#define USER_BUTTON_EXTI_PORT_SOURCE   EXTI_PortSourceGPIOA
#define USER_BUTTON_EXTI_PIN_SOURCE    EXTI_PinSource0
#define USER_BUTTON_EXTI_IRQn          EXTI0_1_IRQn

    /** @addtogroup STM32F0-DISCOVERY_LOW_LEVEL_COM
     * @{
     */
#define COMn                             1

    /**
    * @brief Definition for COM port1, connected to USART1
    */
#define EVAL_COM1                        USART1
#define EVAL_COM1_CLK                    RCC_APB2Periph_USART1

#define EVAL_COM1_TX_PIN                 GPIO_Pin_9
#define EVAL_COM1_TX_GPIO_PORT           GPIOA
#define EVAL_COM1_TX_GPIO_CLK            RCC_AHBPeriph_GPIOA
#define EVAL_COM1_TX_SOURCE              GPIO_PinSource9
#define EVAL_COM1_TX_AF                  GPIO_AF_1

#define EVAL_COM1_RX_PIN                 GPIO_Pin_10
#define EVAL_COM1_RX_GPIO_PORT           GPIOA
#define EVAL_COM1_RX_GPIO_CLK            RCC_AHBPeriph_GPIOA
#define EVAL_COM1_RX_SOURCE              GPIO_PinSource10
#define EVAL_COM1_RX_AF                  GPIO_AF_1

#define EVAL_COM1_CTS_PIN                GPIO_Pin_11
#define EVAL_COM1_CTS_GPIO_PORT          GPIOA
#define EVAL_COM1_CTS_GPIO_CLK           RCC_AHBPeriph_GPIOA
#define EVAL_COM1_CTS_SOURCE             GPIO_PinSource11
#define EVAL_COM1_CTS_AF                 GPIO_AF_1

#define EVAL_COM1_RTS_PIN                GPIO_Pin_12
#define EVAL_COM1_RTS_GPIO_PORT          GPIOA
#define EVAL_COM1_RTS_GPIO_CLK           RCC_AHBPeriph_GPIOA
#define EVAL_COM1_RTS_SOURCE             GPIO_PinSource12
#define EVAL_COM1_RTS_AF                 GPIO_AF_1

#define EVAL_COM1_IRQn                   USART1_IRQn

    /**
      * @}
      */


    /** @defgroup STM32F0_DISCOVERY_LOW_LEVEL_Exported_Macros
      * @{
      */
    /**
      * @}
      */

    /** @defgroup STM32F0_DISCOVERY_LOW_LEVEL_Exported_Functions
      * @{
      */
    void STM_EVAL_LEDInit(Led_TypeDef Led);
    void STM_EVAL_LEDOn(Led_TypeDef Led);
    void STM_EVAL_LEDOff(Led_TypeDef Led);
    void STM_EVAL_LEDToggle(Led_TypeDef Led);
    void STM_EVAL_PBInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
    uint32_t STM_EVAL_PBGetState(Button_TypeDef Button);
    void STM_EVAL_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct);

    /**
      * @}
      */

    extern GPIO_TypeDef* GPIO_PORT[LEDn];
    extern const uint16_t GPIO_PIN[LEDn];

#ifdef __cplusplus
}
#endif

#endif /* __STM32F0_DISCOVERY_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
