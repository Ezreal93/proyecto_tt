#ifndef HLD_SPI_H
#define HLD_SPI_H

#include <stdint.h>

void spi1_init(void);
uint16_t spi1_xfer(uint8_t* txdata, uint8_t txdata_len, uint8_t* rxdata, uint8_t rxdata_len);

#endif
