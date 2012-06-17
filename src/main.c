#include "stm32f0xx_conf.h"

void SysTick_Handler(void) {
  static uint16_t tick = 0;

  switch (tick++) {
  	case 100:
  		tick = 0;
  		GPIOC->ODR ^= (1 << 8);
  		break;
  }
}

int main(void)
{

	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 	// enable the clock to GPIOC
						//(RM0091 lists this as IOPCEN, not GPIOCEN)
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // enable TIM2 clock

	GPIOC->MODER = (1 << 16);

	SysTick_Config(SystemCoreClock/100);

	while(1);

}
