#ifndef HDL_API_H
#define HDL_API_H

#include <stdint.h>

typedef struct {
    void (*sendData)(uint8_t  address_7b, uint8_t data);
		uint8_t (*readData)(uint8_t address_7b, uint8_t regAddress);
    int (*sendPacket)(uint8_t address, uint8_t *data, uint8_t nbytes);
    int (*readPacket)(uint8_t address, uint8_t internal_address, uint8_t *data, uint8_t nbytes);
} hld_i2c_t;

#endif
