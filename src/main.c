#include "stm32f0xx_conf.h"

void SysTick_Handler(void) {
  static uint16_t tick = 0;

  switch (tick++) {
    case 100:
        tick = 0;
        GPIOC->ODR ^= (1 << 8) | (1 << 9);
        break;
  }
}

int main(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;  // enable the clock to GPIOC
                        //(RM0091 lists this as IOPCEN, not GPIOCEN)

    GPIOC->MODER = (1 << 16) | (1 << 18);
    GPIOC->ODR |= (1 << 8);

    SysTick_Config(SystemCoreClock/100);

    while(1);

}
