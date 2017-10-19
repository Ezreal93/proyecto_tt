#ifndef HDL_I2C_H
#define HDL_I2C_H

#include <stdint.h>

void init_i2c1(void);
int i2c1_write(uint8_t address, uint8_t nbytes, uint8_t *data);
int i2c1_read(uint8_t address, uint8_t internal_address, uint8_t nbytes, uint8_t *data);

#endif
