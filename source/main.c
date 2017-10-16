#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_bus.h"

int main(){
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_GPIO_InitTypeDef mygpio;
  LL_GPIO_StructInit(&mygpio);
  return 0;
}
