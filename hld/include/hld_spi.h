#ifndef HLD_SPI_H
#define HLD_SPI_H

#include <stdint.h>

void spi1_init(void);
uint16_t spi1_xfer16(uint16_t data);

#endif
