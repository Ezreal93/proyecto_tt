#ifndef COM_MODULE_H_INCLUDED
#define COM_MODULE_H_INCLUDED

#include "hld_api.h"

#define ERR_NONE                        0x00
#define ERR_CHIP_NOT_FOUND              0x01

#define ERR_PACKET_TOO_LONG             0x10

#define ERR_RX_TIMEOUT                  0x20
#define ERR_CRC_MISMATCH                0x21

#define ERR_INVALID_BANDWIDTH           0x30
#define ERR_INVALID_SPREADING_FACTOR    0x31
#define ERR_INVALID_CODING_RATE         0x32

#define ERR_INVALID_BIT_RANGE           0x40


uint8_t commodule_init(hld_spi_t* spi);

uint8_t commodule_getRegValue(uint8_t reg, uint8_t msb, uint8_t lsb);
uint8_t commodule_readRegisterBurst(uint8_t reg, uint8_t numBytes, uint8_t* inBytes);
uint8_t commodule_readRegisterBurstStr(uint8_t reg, uint8_t numBytes, char* str);
uint8_t commodule_readRegister(uint8_t reg);

uint8_t commodule_setRegValue(uint8_t reg, uint8_t value, uint8_t msb, uint8_t lsb);
void commodule_writeRegisterBurst(uint8_t reg, uint8_t* data, uint8_t numBytes);
void commodule_writeRegisterBurstStr(uint8_t reg, const char* data, uint8_t numBytes);
void commodule_writeRegister(uint8_t reg, uint8_t data);

#endif
