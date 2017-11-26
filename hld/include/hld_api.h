#ifndef HDL_API_H
#define HDL_API_H

#include <stdint.h>

typedef struct {
    int8_t (*sendData)(uint8_t  address_7b, uint8_t data);
		int8_t (*readData)(uint8_t address_7b, uint8_t regAddress);
    int8_t (*sendPacket)(uint8_t address, uint8_t *data, uint8_t nbytes);
    int8_t (*readPacket)(uint8_t address, uint8_t internal_address, uint8_t *data, uint8_t nbytes);
		int8_t (*send_packetAtRegister)(uint8_t address, uint8_t regAddress, uint8_t *data, uint8_t nbytes);
} hld_i2c_t;

#endif
