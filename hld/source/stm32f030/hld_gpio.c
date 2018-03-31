#include "hld_gpio.h"
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_bus.h"
#include <stddef.h>
#include <stdlib.h>

void gpioa_pin_ro_init(uint8_t pin){
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_GPIO_InitTypeDef gpio;
    LL_GPIO_StructInit(&gpio);
    gpio.Pin = (0x01 << pin);
    gpio.Mode = LL_GPIO_MODE_INPUT;
    gpio.Pull = LL_GPIO_PULL_UP;
    LL_GPIO_Init(GPIOA, &gpio);
}
// PIN A0

int8_t read_pa0(void){
    return LL_GPIO_IsInputPinSet(GPIOA, 0x1 << 0);
}

hld_pin_ro_t*  hld_gpio_a0_ro_init(void){
    //init
    hld_pin_ro_t* pin_ro;
    gpioa_pin_ro_init(0);
    pin_ro =  malloc(sizeof(hld_pin_ro_t));
    pin_ro->read = read_pa0;
    return pin_ro;
}

// PIN A1

int8_t read_pa1(void){
    return LL_GPIO_IsInputPinSet(GPIOA, 0x1 << 1);
}

hld_pin_ro_t*  hld_gpio_a1_ro_init(void){
    hld_pin_ro_t* pin_ro;
    gpioa_pin_ro_init(1);
    pin_ro =  malloc(sizeof(hld_pin_ro_t));
    pin_ro->read = read_pa1;
    return pin_ro;
}

// Write only init
void gpioa_pin_wo_init(uint8_t pin){
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_GPIO_InitTypeDef gpio;
    LL_GPIO_StructInit(&gpio);
    gpio.Pin = (0x01 << pin);
    gpio.Mode = LL_GPIO_MODE_OUTPUT;
    LL_GPIO_Init(GPIOA, &gpio);
}

// PIN A9

void write_pa9(int8_t state){
    LL_GPIO_SetOutputPin(GPIOA, (state ? 0x1 << 9 : (0x1 << (16 + 9))));
}

hld_pin_wo_t*  hld_gpio_a9_wo_init(void){
    hld_pin_wo_t* pin_wo;
    gpioa_pin_wo_init(9);
    pin_wo =  malloc(sizeof(hld_pin_wo_t));
    pin_wo->write = write_pa9;
    return pin_wo;
}

// PIN A10

void write_pa10(int8_t state){
     LL_GPIO_SetOutputPin(GPIOA, (state ? 0x1 << 10 : (0x1 << (16 + 10))));
}

hld_pin_wo_t*  hld_gpio_a10_wo_init(void){
    hld_pin_wo_t* pin_wo;
    gpioa_pin_wo_init(10);
    pin_wo =  malloc(sizeof(hld_pin_wo_t));
    pin_wo->write = write_pa10;
    return pin_wo;
}

// PORTB
// Write only init !!open drain!!
void gpiob_pin_wo_init(uint8_t pin){
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    LL_GPIO_InitTypeDef gpio;
    LL_GPIO_StructInit(&gpio);
    gpio.Pin = (0x01 << pin);
    gpio.Mode = LL_GPIO_MODE_OUTPUT;
    gpio.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    gpio.Pull = LL_GPIO_PULL_UP;
    LL_GPIO_Init(GPIOB, &gpio);
}


// PIN B1
void write_pb1(int8_t state){
     LL_GPIO_SetOutputPin(GPIOB, (state ? 0x1 << 1 : (0x1 << (16 + 1))));
}

hld_pin_wo_t*  hld_gpio_b1_wo_init(void){
    hld_pin_wo_t* pin_wo;
    gpiob_pin_wo_init(1);
    pin_wo =  malloc(sizeof(hld_pin_wo_t));
    pin_wo->write = write_pb1;
    return pin_wo;
}
