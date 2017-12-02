#include "hld_spi.h"

#include <stdbool.h>

#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_spi.h"

uint16_t spi1_xfer(uint8_t* txdata, uint8_t txdata_len, uint8_t* rxdata, uint8_t rxdata_len);

hld_spi_t spi1;

hld_spi_t* spi1_init(void){
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_GPIO_InitTypeDef gpio;
    LL_GPIO_StructInit(&gpio);
    gpio.Pin = LL_GPIO_PIN_5 | LL_GPIO_PIN_6 | LL_GPIO_PIN_7;
    gpio.Mode = LL_GPIO_MODE_ALTERNATE;
    gpio.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    gpio.Alternate = LL_GPIO_AF_0;
    LL_GPIO_Init(GPIOA, &gpio);

    gpio.Pin = LL_GPIO_PIN_4;
    gpio.Mode = LL_GPIO_MODE_OUTPUT;
    gpio.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    LL_GPIO_Init(GPIOA, &gpio);

    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_4);

    LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SPI1);

    LL_SPI_InitTypeDef spi;
    LL_SPI_StructInit(&spi);
    spi.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV8;
    spi.BitOrder = LL_SPI_MSB_FIRST;
    spi.ClockPhase = LL_SPI_PHASE_1EDGE;
    spi.ClockPolarity = LL_SPI_POLARITY_LOW;
    spi.DataWidth = LL_SPI_DATAWIDTH_8BIT;
    spi.Mode = LL_SPI_MODE_MASTER;
    spi.TransferDirection = LL_SPI_FULL_DUPLEX;
    spi.NSS = LL_SPI_NSS_SOFT;
    LL_SPI_Init(SPI1, &spi);
    //
    spi1.xfer = spi1_xfer;
    return &spi1;
}

uint16_t spi1_xfer(uint8_t* txdata, uint8_t txdata_len, uint8_t* rxdata, uint8_t rxdata_len){

    LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_4);//CS LOW
    LL_SPI_Enable(SPI1);
    uint8_t read_counter = 0;
    uint8_t write_counter = 0;

    //empty rx buffer before transmission
    while(LL_SPI_IsActiveFlag_RXNE(SPI1)){
        LL_SPI_ReceiveData8(SPI1);
    }

    //transmit all data
    while(read_counter < txdata_len){
        if(LL_SPI_IsActiveFlag_TXE(SPI1) && (write_counter < txdata_len)){
            LL_SPI_TransmitData8(SPI1, txdata[write_counter]);
            write_counter++;
        }

        if(LL_SPI_IsActiveFlag_RXNE(SPI1)){
            LL_SPI_ReceiveData8(SPI1);
            read_counter++;
        }
    }

    read_counter = 0;
    write_counter = 0;

    while(read_counter < rxdata_len){
        if(LL_SPI_IsActiveFlag_TXE(SPI1) && (write_counter < rxdata_len)){
            LL_SPI_TransmitData8(SPI1, 0x00);
            write_counter++;
        }

        if(LL_SPI_IsActiveFlag_RXNE(SPI1)){
            rxdata[read_counter] = LL_SPI_ReceiveData8(SPI1);
            read_counter++;
        }
    }

    LL_SPI_Disable(SPI1);
    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_4);//CS HIGH

    return 0;
}
