#include "stm32f0xx_conf.h"
#include "test.h"

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
    volatile int u = fff(8, 8);

    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;  // enable the clock to GPIOC
                        //(RM0091 lists this as IOPCEN, not GPIOCEN)

    GPIOC->MODER = (1 << 16);

    SysTick_Config(SystemCoreClock/100);

    while(1);

}
