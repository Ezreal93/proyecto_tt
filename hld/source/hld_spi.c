#include "hld_spi.h"

#include <stdbool.h>

#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_spi.h"

void spi1_init(void){
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_GPIO_InitTypeDef gpio;
    LL_GPIO_StructInit(&gpio);
    gpio.Pin = LL_GPIO_PIN_5 | LL_GPIO_PIN_6 | LL_GPIO_PIN_7;
    gpio.Mode = LL_GPIO_MODE_ALTERNATE;
    gpio.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    gpio.Alternate = LL_GPIO_AF_0;
    LL_GPIO_Init(GPIOA, &gpio);

    gpio.Pin = LL_GPIO_PIN_4;
    gpio.Mode = LL_GPIO_MODE_ALTERNATE;
    gpio.Alternate = LL_GPIO_AF_0;
    gpio.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    LL_GPIO_Init(GPIOA, &gpio);

    LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SPI1);

    LL_SPI_InitTypeDef spi;
    LL_SPI_StructInit(&spi);
    spi.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV8;
    spi.BitOrder = LL_SPI_MSB_FIRST;
    spi.ClockPhase = LL_SPI_PHASE_1EDGE;
    spi.ClockPolarity = LL_SPI_POLARITY_LOW;
    spi.DataWidth = LL_SPI_DATAWIDTH_16BIT;
    spi.Mode = LL_SPI_MODE_MASTER;
    spi.TransferDirection = LL_SPI_FULL_DUPLEX;
    spi.NSS = LL_SPI_NSS_HARD_OUTPUT;
    LL_SPI_Init(SPI1, &spi);
    LL_SPI_EnableNSSPulseMgt(SPI1);

    LL_SPI_Enable(SPI1);
}

uint16_t spi1_xfer16(uint16_t data){
    LL_SPI_TransmitData16(SPI1, data);
    while(LL_SPI_IsActiveFlag_TXE(SPI1)){}//wait until empty TX
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)){}//wait until not empty RX
    return LL_SPI_ReceiveData16(SPI1);
}
