/**
  ******************************************************************************
  * @file    SysTick/main.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "conf.h"
#include <stdio.h>
#include "usart.h"
#include <stdlib.h>
#include "conio.h"
#include "cli.h"
#include "wifi.h"

/** @addtogroup STM32F0_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup SysTick_Example
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TIMEOUT_0_DEFAULT 50
#define LINEBUFFERSIZE 100

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStructure;
static __IO uint32_t TimingDelay;
static __IO uint8_t timeout_0 = TIMEOUT_0_DEFAULT;
char line_buffer[LINEBUFFERSIZE];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */

  /* Initialize Leds mounted on STM32F0-discovery */
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

  /* Turn on LED3 and LED4 */
  STM_EVAL_LEDOn(LED3);
  STM_EVAL_LEDOn(LED4);

  /* Setup SysTick Timer for 1 msec interrupts.
     ------------------------------------------
    1. The SysTick_Config() function is a CMSIS function which configure:
       - The SysTick Reload register with value passed as function parameter.
       - Configure the SysTick IRQ priority to the lowest value (0x0F).
       - Reset the SysTick Counter register.
       - Configure the SysTick Counter clock source to be Core Clock Source (HCLK).
       - Enable the SysTick Interrupt.
       - Start the SysTick Counter.

    2. You can change the SysTick Clock source to be HCLK_Div8 by calling the
       SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8) just after the
       SysTick_Config() function call. The SysTick_CLKSourceConfig() is defined
       inside the stm32f0xx_misc.c file.

    3. You can change the SysTick IRQ priority by calling the
       NVIC_SetPriority(SysTick_IRQn,...) just after the SysTick_Config() function
       call. The NVIC_SetPriority() is defined inside the core_cm0.h file.

    4. To adjust the SysTick time base, use the following formula:

         Reload Value = SysTick Counter Clock (Hz) x  Desired Time base (s)

       - Reload Value is the parameter to be passed for SysTick_Config() function
       - Reload Value should not exceed 0xFFFFFF
   */
  if (SysTick_Config(SystemCoreClock / 1000))
  {
    /* Capture error */
    while (1);
  }

  Usart1Init();

#ifndef WIFI_CONNECTED
  /* Output a message on Hyperterminal using printf function */
  cio_printf("%s", welcome_msg);
#else
  wifi_intit_pins();
  wifi_init_ap_mode();
#endif

  while (1)
  {    
      // Everything should be non-blocking
      
#ifndef WIFI_CONNECTED 
      if(serve_command_promt(line_buffer, LINEBUFFERSIZE, "microcli> ") > 0)
      {
          shell_process(line_buffer);
      }
#else
      if(get_wifi_msg(line_buffer, LINEBUFFERSIZE, &cid) > 0)
      {
          bool add_header = (cid != -1); // send back to the current connection
          if(add_header)
          {
              cio_printf("\x1bS%c", cid);
              cio_printf("microcli> %s\r\n", line_buffer);
          }
          shell_process(line_buffer);
          
          if(add_header)
          {
              cio_print("[end]\r\n");
              cio_print("\x1b" "E");
          }
      }
#endif

      if(timeout_0 == 0)
      {
          static uint8_t counter = 0;
          switch(counter++) {
          case 0:
              /* Toggle LED4 */
              STM_EVAL_LEDToggle(LED4);
              break;
          case 1:
              /* Toggle LED3 */
              STM_EVAL_LEDToggle(LED3);
              break;
          case 2:
              break;
          default:
              counter = 0;
          }
          timeout_0 = TIMEOUT_0_DEFAULT;
      }
  }
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void delay_ms(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void SystemTick(void)
{
  if(TimingDelay != 00)
  {
    TimingDelay--;
  }
  if(timeout_0 != 00)
  {
      timeout_0--;
  }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
