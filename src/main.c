#include "stm32f0xx_conf.h"

int main(void)
{

//#elif CONFIG_STM32F0_DISCOVERY
//
//#define GPIOC 0x48000800 /* port C */
//#define GPIOC_MODER (GPIOC + 0x00) /* port mode register */
//#define LED_PORT_ODR (GPIOC + 0x14) /* port output data register */
//
//#define LED_BLUE (1 << 8) /* port C, pin 8 */
//#define LED_GREEN (1 << 9) /* port C, pin 9 */
//#define LED_ORANGE 0
//#define LED_RED 0

	GPIOC->MODER = (1 << 16);
	
	while(1) GPIOC->ODR ^= (1 << 16);
	
}
