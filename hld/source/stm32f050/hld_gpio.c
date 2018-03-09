#include "hld_gpio.h"
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_bus.h"
#include <stddef.h>

int8_t read_pb8(void){
    return LL_GPIO_IsOutputPinSet(GPIOB, 8);
}

int8_t read_pb9(void){
    return LL_GPIO_IsOutputPinSet(GPIOB, 9);
}

hld_pin_ro_t pin_b8;
hld_pin_ro_t pin_b9;

void pin_init(uint8_t pin){
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    LL_GPIO_InitTypeDef gpio;
    LL_GPIO_StructInit(&gpio);
    gpio.Pin = (0x01 << pin);
    gpio.Mode = LL_GPIO_MODE_INPUT;
    LL_GPIO_Init(GPIOB, &gpio);
}

hld_pin_ro_t* gpiob_init(uint8_t pin){
    pin_init(pin);
    if(pin == 8){
        pin_b8.read = read_pb8;
        return &pin_b8;
    }else if(pin == 9){
        pin_b9.read = read_pb9;
        return &pin_b9;
    }else{
        return NULL;
    }
}
