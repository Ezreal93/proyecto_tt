#include "hld_i2c.h"

#include <stdbool.h>

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

static uint8_t bytes_remaining = 0;
static uint8_t *p_data;
static uint8_t transfer_in_progress = 0;

int i2c1_write(uint8_t address, uint8_t nbytes, uint8_t *data){
    LL_I2C_SetSlaveAddr(I2C1, address << 1);
    LL_I2C_SetTransferSize(I2C1, nbytes);
    LL_I2C_EnableAutoEndMode(I2C1);
    LL_I2C_SetTransferRequest(I2C1,  LL_I2C_REQUEST_WRITE);

    bytes_remaining = nbytes;
    p_data = data;
    transfer_in_progress = true;


    LL_I2C_EnableIT_TX(I2C1);
    NVIC_EnableIRQ(I2C1_IRQn);

    LL_I2C_GenerateStartCondition(I2C1);

    while(transfer_in_progress){};

    return 0;
}

int i2c1_read(uint8_t address, uint8_t internal_address, uint8_t nbytes, uint8_t *data){
    LL_I2C_SetSlaveAddr(I2C1, address << 1);
    LL_I2C_SetTransferSize(I2C1, 1);
    LL_I2C_DisableAutoEndMode(I2C1);
    LL_I2C_SetTransferRequest(I2C1,  LL_I2C_REQUEST_WRITE);

    bytes_remaining = nbytes;
    p_data = data;
    transfer_in_progress = true;

    while(!LL_I2C_IsActiveFlag_TXIS(I2C1));
    LL_I2C_TransmitData8(I2C1, internal_address);

    while(!LL_I2C_IsActiveFlag_TC(I2C1));
    LL_I2C_GenerateStartCondition(I2C1);
    LL_I2C_SetTransferSize(I2C1, nbytes);
    LL_I2C_EnableAutoEndMode(I2C1);
    LL_I2C_SetTransferRequest(I2C1,  LL_I2C_REQUEST_READ);

    LL_I2C_EnableIT_RX(I2C1);
    NVIC_EnableIRQ(I2C1_IRQn);

    LL_I2C_GenerateStartCondition(I2C1);

    while(transfer_in_progress){};

    return 0;
}


void I2C1_IRQHandler(void){
    if(LL_I2C_IsActiveFlag_TXIS(I2C1)){
        LL_I2C_TransmitData8(I2C1, *p_data);
        p_data++;
        bytes_remaining--;
        if(!bytes_remaining){
            transfer_in_progress = false;
            LL_I2C_DisableIT_TX(I2C1);
            NVIC_DisableIRQ(I2C1_IRQn);
        }
    }
    if(LL_I2C_IsActiveFlag_RXNE(I2C1)){
        *p_data = LL_I2C_ReceiveData8(I2C1);
        p_data++;
        bytes_remaining--;
        if(!bytes_remaining){
            transfer_in_progress = false;
            LL_I2C_DisableIT_RX(I2C1);
            NVIC_DisableIRQ(I2C1_IRQn);
        }
    }
}
