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

typedef struct {
    int8_t (*writePacket)(uint8_t *data, uint8_t nbytes);
    int8_t (*readPacket)(uint8_t *data, uint8_t nbytes);
} hld_spi_t;

#define ON_RISING_EDGE (0x1 << 0)
#define ON_FALLIN_EDGE (0x1 << 1)
#define ON_BOTH_EDGES (ON_RISING_EDGE | ON_FALLIN_EDGE)

typedef struct {
    int8_t (*read)(void);
    int8_t (*event)(int8_t options, void (*callback)(int8_t state));
} hld_pin_ro_t;

#endif
