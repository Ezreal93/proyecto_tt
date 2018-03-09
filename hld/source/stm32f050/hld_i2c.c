#include "hld_i2c.h"

#include <stdbool.h>
#include <string.h>

#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_i2c.h"

hld_i2c_t i2c1_struct;

int8_t i2c1_sendData(uint8_t  address_7b, uint8_t data);
int8_t i2c1_readData(uint8_t address_7b, uint8_t regAddress);
int8_t i2c1_sendPacket(uint8_t address,  uint8_t *data, uint8_t nbytes);
int8_t i2c1_readPacket(uint8_t address, uint8_t internal_address, uint8_t *data, uint8_t nbytes);
int8_t i2c1_send_packetAtRegister(uint8_t address, uint8_t regAddress, uint8_t *data, uint8_t nbytes);

hld_i2c_t* i2c1_init(void){
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

    i2c1_struct.readData = i2c1_readData;
    i2c1_struct.sendData = i2c1_sendData;
    i2c1_struct.readPacket = i2c1_readPacket;
    i2c1_struct.sendPacket = i2c1_sendPacket;
    i2c1_struct.send_packetAtRegister = i2c1_send_packetAtRegister;

    return &i2c1_struct;
 }

#define I2C_WRITE 0
#define I2C_READ 1

static uint8_t bytes_remaining = 0;
static uint8_t *p_data;
static uint8_t transfer_in_progress = 0;
static uint8_t xfer_direction = 0;

int8_t i2c1_sendData(uint8_t  address_7b, uint8_t data){
    return i2c1_sendPacket(address_7b,  &data, 1);
}

int8_t i2c1_readData(uint8_t address_7b, uint8_t regAddress){
    uint8_t data;
    i2c1_readPacket(address_7b, regAddress, &data, 1);
    return data;
}

int8_t i2c1_sendPacket(uint8_t address,  uint8_t *data, uint8_t nbytes){
    xfer_direction =  I2C_WRITE;
    bytes_remaining = nbytes;
    p_data = data;
    transfer_in_progress = true;

    LL_I2C_HandleTransfer(I2C1, (address << 1), LL_I2C_ADDRSLAVE_7BIT, nbytes, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);
    LL_I2C_EnableIT_TX(I2C1);
    NVIC_EnableIRQ(I2C1_IRQn);

    while(transfer_in_progress){};

    return 0;
}

/* Do not use with DMA or any other async method!! */
int8_t i2c1_send_packetAtRegister(uint8_t address, uint8_t regAddress, uint8_t *data, uint8_t nbytes){
    uint8_t tempBuffer[nbytes+1];
    tempBuffer[0] = regAddress;
    memcpy(tempBuffer+1, data, nbytes);
    return i2c1_sendPacket(address,  tempBuffer, nbytes+1);
}

int8_t i2c1_readPacket(uint8_t address, uint8_t internal_address, uint8_t *data, uint8_t nbytes){
    LL_I2C_SetSlaveAddr(I2C1, address << 1);
    LL_I2C_SetTransferSize(I2C1, 1);
    LL_I2C_DisableAutoEndMode(I2C1);
    LL_I2C_SetTransferRequest(I2C1,  LL_I2C_REQUEST_WRITE);

    xfer_direction =  I2C_READ;
    bytes_remaining = nbytes;
    p_data = data;
    transfer_in_progress = true;

    LL_I2C_GenerateStartCondition(I2C1);
    while(!LL_I2C_IsActiveFlag_TXIS(I2C1));
    LL_I2C_TransmitData8(I2C1, internal_address);


    while(!LL_I2C_IsActiveFlag_TC(I2C1));
    LL_I2C_SetTransferSize(I2C1, nbytes);
    LL_I2C_EnableAutoEndMode(I2C1);
    LL_I2C_SetTransferRequest(I2C1,  LL_I2C_REQUEST_READ);
    LL_I2C_GenerateStartCondition(I2C1);

    LL_I2C_EnableIT_RX(I2C1);
    NVIC_EnableIRQ(I2C1_IRQn);

    while(transfer_in_progress){};

    return 0;
}


void I2C1_IRQHandler(void){
    if(LL_I2C_IsActiveFlag_TXIS(I2C1) && (xfer_direction == I2C_WRITE)){
        LL_I2C_TransmitData8(I2C1, *p_data);
        p_data++;
        bytes_remaining--;
        if(!bytes_remaining){
            transfer_in_progress = false;
            LL_I2C_DisableIT_TX(I2C1);
            NVIC_DisableIRQ(I2C1_IRQn);
        }
    }
    if(LL_I2C_IsActiveFlag_RXNE(I2C1) && (xfer_direction == I2C_READ)){
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
