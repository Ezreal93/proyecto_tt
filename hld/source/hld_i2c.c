#include "hld_i2c.h"

#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_i2c.h"

void init_i2c1(void){
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    LL_GPIO_InitTypeDef gpio;
    LL_GPIO_StructInit(&gpio);
    gpio.Pin = LL_GPIO_PIN_6 | LL_GPIO_PIN_7;
    gpio.Mode = LL_GPIO_MODE_ALTERNATE;
    gpio.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    gpio.Alternate = LL_GPIO_AF_1;
    LL_GPIO_Init(GPIOB, &gpio);
  
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);
    LL_I2C_InitTypeDef i2c;
    LL_I2C_StructInit(&i2c);
    i2c.PeripheralMode = LL_I2C_MODE_I2C;
    i2c.Timing = __LL_I2C_CONVERT_TIMINGS(0x1, 0x4, 0x2, 0xf, 0x13);
    LL_I2C_Init(I2C1, &i2c);
    LL_I2C_Enable(I2C1);
 }
