/**
  ******************************************************************************
  * @file    stm32f0_discovery.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   This file provides set of firmware functions to manage Leds and
  *          push-button available on STM32F0-DISCOVERY Kit from STMicroelectronics.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  * <h2><center>&copy; COPYRIGHT 2012 Christian Jann</center></h2>
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f0_discovery.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32F0_DISCOVERY
  * @{
  */

/** @defgroup STM32F0_DISCOVERY_LOW_LEVEL
  * @brief This file provides firmware functions to manage Leds and push-buttons,
  *        available on STM32F0_DISCOVERY evaluation board from STMicroelectronics.
  * @{
  */

/** @defgroup STM32F0_DISCOVERY_LOW_LEVEL_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */


/** @defgroup STM32F0_DISCOVERY_LOW_LEVEL_Private_Defines
  * @{
  */
/**
  * @}
  */


/** @defgroup STM32F0_DISCOVERY_LOW_LEVEL_Private_Macros
  * @{
  */
/**
  * @}
  */


/** @defgroup STM32F0_DISCOVERY_LOW_LEVEL_Private_Variables
  * @{
  */
GPIO_TypeDef* GPIO_PORT[LEDn] = {LED3_GPIO_PORT, LED4_GPIO_PORT,
                                 RELAIS1_GPIO_PORT, RELAIS2_GPIO_PORT,
                                 OUT_12V_1_GPIO_PORT, OUT_12V_2_GPIO_PORT
                                };
const uint16_t GPIO_PIN[LEDn] = {LED3_PIN, LED4_PIN,
                                 RELAIS1_PIN, RELAIS2_PIN,
                                 OUT_12V_1_PIN, OUT_12V_2_PIN
                                };

const uint32_t GPIO_CLK[LEDn] = {LED3_GPIO_CLK, LED4_GPIO_CLK,
                                 RELAIS1_GPIO_CLK, RELAIS2_GPIO_CLK,
                                 OUT_12V_1_GPIO_CLK, OUT_12V_2_GPIO_CLK
                                };

GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {USER_BUTTON_GPIO_PORT, IN_12V_1_GPIO_PORT,
                                      IN_12V_2_GPIO_PORT
                                     };

const uint16_t BUTTON_PIN[BUTTONn] = {USER_BUTTON_PIN, IN_12V_1_PIN, IN_12V_2_PIN};

const uint32_t BUTTON_CLK[BUTTONn] = {USER_BUTTON_GPIO_CLK, IN_12V_1_GPIO_CLK,
                                      IN_12V_2_GPIO_CLK
                                     };

const uint16_t BUTTON_EXTI_LINE[BUTTONn] = {USER_BUTTON_EXTI_LINE};

const uint16_t BUTTON_PORT_SOURCE[BUTTONn] = {USER_BUTTON_EXTI_PORT_SOURCE};

const uint16_t BUTTON_PIN_SOURCE[BUTTONn] = {USER_BUTTON_EXTI_PIN_SOURCE};

const uint16_t BUTTON_IRQn[BUTTONn] = {USER_BUTTON_EXTI_IRQn};

USART_TypeDef* COM_USART[COMn] = {EVAL_COM1};

GPIO_TypeDef* COM_TX_PORT[COMn] = {EVAL_COM1_TX_GPIO_PORT};

GPIO_TypeDef* COM_RX_PORT[COMn] = {EVAL_COM1_RX_GPIO_PORT};

const uint32_t COM_USART_CLK[COMn] = {EVAL_COM1_CLK};

const uint32_t COM_TX_PORT_CLK[COMn] = {EVAL_COM1_TX_GPIO_CLK};

const uint32_t COM_RX_PORT_CLK[COMn] = {EVAL_COM1_RX_GPIO_CLK};

const uint16_t COM_TX_PIN[COMn] = {EVAL_COM1_TX_PIN};

const uint16_t COM_RX_PIN[COMn] = {EVAL_COM1_RX_PIN};

const uint16_t COM_TX_PIN_SOURCE[COMn] = {EVAL_COM1_TX_SOURCE};

const uint16_t COM_RX_PIN_SOURCE[COMn] = {EVAL_COM1_RX_SOURCE};

const uint16_t COM_TX_AF[COMn] = {EVAL_COM1_TX_AF};

const uint16_t COM_RX_AF[COMn] = {EVAL_COM1_RX_AF};

/**
  * @}
  */


/** @defgroup STM32F0_DISCOVERY_LOW_LEVEL_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */

/** @defgroup STM32F0_DISCOVERY_LOW_LEVEL_Private_Functions
  * @{
  */

/**
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured.
  *   This parameter can be one of following parameters:
  *     @arg LED3
  *     @arg LED4
  * @retval None
  */
void STM_EVAL_LEDInit(Led_TypeDef Led)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    /* Enable the GPIO_LED Clock */
    RCC_AHBPeriphClockCmd(GPIO_CLK[Led], ENABLE);

    /* Configure the GPIO_LED pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN[Led];
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIO_PORT[Led], &GPIO_InitStructure);
}

/**
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on.
  *   This parameter can be one of following parameters:
  *     @arg LED3
  *     @arg LED4
  * @retval None
  */
void STM_EVAL_LEDOn(Led_TypeDef Led)
{
    GPIO_PORT[Led]->BSRR = GPIO_PIN[Led];
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led: Specifies the Led to be set off.
  *   This parameter can be one of following parameters:
  *     @arg LED3
  *     @arg LED4
  * @retval None
  */
void STM_EVAL_LEDOff(Led_TypeDef Led)
{
    GPIO_PORT[Led]->BRR = GPIO_PIN[Led];
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led: Specifies the Led to be toggled.
  *   This parameter can be one of following parameters:
  *     @arg LED3
  *     @arg LED4
  * @retval None
  */
void STM_EVAL_LEDToggle(Led_TypeDef Led)
{
    GPIO_PORT[Led]->ODR ^= GPIO_PIN[Led];
}

/**
  * @brief  Configures Button GPIO and EXTI Line.
  * @param  Button: Specifies the Button to be configured.
  *   This parameter can be:
  *     @arg BUTTON_USER: User Push Button
  * @param  Button_Mode: Specifies Button mode.
  *   This parameter can be one of following parameters:
  *     @arg BUTTON_MODE_GPIO: Button will be used as simple IO
  *     @arg BUTTON_MODE_EXTI: Button will be connected to EXTI line with interrupt
  *                            generation capability
  * @retval None
  */
void STM_EVAL_PBInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the BUTTON Clock */
    RCC_AHBPeriphClockCmd(BUTTON_CLK[Button], ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    /* Configure Button pin as input */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    if (Button_Mode == BUTTON_MODE_PDOWN)
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    else
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin = BUTTON_PIN[Button];
    GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStructure);

    if (Button_Mode == BUTTON_MODE_EXTI)
    {
        /* Connect Button EXTI Line to Button GPIO Pin */
        SYSCFG_EXTILineConfig(BUTTON_PORT_SOURCE[Button], BUTTON_PIN_SOURCE[Button]);

        /* Configure Button EXTI line */
        EXTI_InitStructure.EXTI_Line = BUTTON_EXTI_LINE[Button];
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//        if (Button != BUTTON_USER)
//        {
            EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//        }
//        else
//        {
//            EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//        }
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        EXTI_Init(&EXTI_InitStructure);

        /* Enable and set Button EXTI Interrupt to the lowest priority */
        NVIC_InitStructure.NVIC_IRQChannel = BUTTON_IRQn[Button];
        NVIC_InitStructure.NVIC_IRQChannelPriority = 0x03;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

        NVIC_Init(&NVIC_InitStructure);
    }
}

/**
  * @brief  Returns the selected Button state.
  * @param  Button: Specifies the Button to be checked.
  *   This parameter can be one of following parameters:
  *     @arg BUTTON_USER: User Push Button
  * @retval The Button GPIO pin value.
  */
uint32_t STM_EVAL_PBGetState(Button_TypeDef Button)
{
    /* There is no Wakeup button on STM32f0-Discovery Kit */
    return GPIO_ReadInputDataBit(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}

/**
  * @brief  Configures COM port.
  * @param  COM: Specifies the COM port to be configured.
  *          This parameter can be one of following parameters:
  *            @arg COM1
  * @param  USART_InitStruct: pointer to a USART_InitTypeDef structure that
  *   contains the configuration information for the specified USART peripheral.
  * @retval None
  */
void STM_EVAL_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIO clock */
    RCC_AHBPeriphClockCmd(COM_TX_PORT_CLK[COM] | COM_RX_PORT_CLK[COM], ENABLE);

    /* Enable USART clock */
    RCC_APB2PeriphClockCmd(COM_USART_CLK[COM], ENABLE);

    /* Connect PXx to USARTx_Tx */
    GPIO_PinAFConfig(COM_TX_PORT[COM], COM_TX_PIN_SOURCE[COM], COM_TX_AF[COM]);

    /* Connect PXx to USARTx_Rx */
    GPIO_PinAFConfig(COM_RX_PORT[COM], COM_RX_PIN_SOURCE[COM], COM_RX_AF[COM]);

    /* Configure USART Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);

    /* Configure USART Rx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
    GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);

    /* USART configuration */
    USART_Init(COM_USART[COM], USART_InitStruct);

    /* Enable USART */
    USART_Cmd(COM_USART[COM], ENABLE);
}

/**
  * @}
  */

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
