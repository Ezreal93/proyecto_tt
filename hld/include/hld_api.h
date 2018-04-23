#ifndef HDL_API_H
#define HDL_API_H

#include <stdint.h>

typedef struct {
    int8_t (*sendData)(uint8_t  address_7bit, uint8_t data);
    int8_t (*readData)(uint8_t address_7bit, uint8_t regAddress);
    int8_t (*sendPacket)(uint8_t address_7bit, uint8_t *data, uint8_t nbytes);
    int8_t (*readPacket)(uint8_t address_7bit, uint8_t internal_address, uint8_t *data, uint8_t nbytes);
    int8_t (*send_packetAtRegister)(uint8_t address_7bit, uint8_t regAddress, uint8_t *data, uint8_t nbytes);
} hld_i2c_t;

typedef struct {
    uint16_t (*xfer)(uint8_t* txdata, uint8_t txdata_len, uint8_t* rxdata, uint8_t rxdata_len);
} hld_spi_t;

#define ON_RISING_EDGE (0x1 << 0)
#define ON_FALLIN_EDGE (0x1 << 1)
#define ON_BOTH_EDGES (ON_RISING_EDGE | ON_FALLIN_EDGE)

typedef struct {
    int8_t (*read)(void);
} hld_pin_ro_t;

typedef struct {
    void (*write)(int8_t);
} hld_pin_wo_t;

#endif
