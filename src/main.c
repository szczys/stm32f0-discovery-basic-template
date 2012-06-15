#include "stm32f0xx_conf.h"

void TIM2_IRQHandler(void) {
  // flash on update event
  if (TIM2->SR & TIM_SR_UIF) GPIOC->ODR ^= (1 << 8);
   
  TIM2->SR = 0x0; // reset the status register
}

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

	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 	// enable the clock to GPIOC 
						//(RM0091 lists this as IOPCEN, not GPIOCEN)
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // enable TIM2 clock

	GPIOC->MODER = (1 << 16);
	
	NVIC->ISER[0] |= 1<< (TIM2_IRQn); // enable the TIM2 IRQ
	
	TIM2->PSC = 0x0; // no prescaler, timer counts up in sync with the peripheral clock
	TIM2->DIER |= TIM_DIER_UIE; // enable update interrupt
	TIM2->ARR = 0x01; // count to 1 (autoreload value 1)
	TIM2->CR1 |= TIM_CR1_ARPE | TIM_CR1_CEN; // autoreload on, counter enabled
	TIM2->EGR = 1; // trigger update event to reload timer registers
	
	while(1);
	
}
