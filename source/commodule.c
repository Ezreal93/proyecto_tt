#include "commodule.h"
#include <string.h>

static hld_spi_t* _spi;
static hld_pin_ro_t* _pin0;
static hld_pin_ro_t* _pin1;

uint8_t module_init(hld_spi_t* spi, hld_pin_ro_t* pin0, hld_pin_ro_t* pin1) {
    _spi = spi;
    _pin0 = pin0;
    _pin1 = pin1;

    return ERR_NONE;
}

uint8_t commodule_getRegValue(uint8_t reg, uint8_t msb, uint8_t lsb) {
    uint8_t rawValue = commodule_readRegister(reg);
    uint8_t maskedValue = rawValue & ((0b11111111 << lsb) & (0b11111111 >> (7 - msb)));
    return(maskedValue);
}

uint8_t commodule_readRegister(uint8_t reg) {
    uint8_t inByte;
    _spi->writePacket(&reg, 1);
    _spi->readPacket(&inByte, 1);
    return(inByte);
}

uint8_t commodule_readRegisterBurst(uint8_t reg, uint8_t numBytes, uint8_t* inBytes) {
    _spi->writePacket(&reg, 1);
    _spi->readPacket(inBytes, numBytes);
    return(ERR_NONE);
}


uint8_t commodule_readRegisterBurstStr(uint8_t reg, uint8_t numBytes, char* inBytes) {
    return commodule_readRegisterBurst(reg, numBytes, (uint8_t*) inBytes);
}

uint8_t commodule_setRegValue(uint8_t reg, uint8_t value, uint8_t msb, uint8_t lsb) {
    uint8_t currentValue = commodule_readRegister(reg);
    uint8_t newValue = currentValue & ((0b11111111 << (msb + 1)) & (0b11111111 >> (8 - lsb)));
    commodule_writeRegister(reg, newValue | value);
    return(ERR_NONE);
}

void commodule_writeRegister(uint8_t reg, uint8_t data) {
    uint8_t tempBuffer[2] = {reg, data};
    _spi->writePacket(tempBuffer, 2);
}

void commodule_writeRegisterBurst(uint8_t reg, uint8_t* data, uint8_t numBytes) {
    uint8_t tempBuffer[numBytes + 1];
    tempBuffer[0] = reg;
    memcpy(tempBuffer, data, numBytes);

    _spi->writePacket(tempBuffer, numBytes + 1);
}

void commodule_writeRegisterBurstStr(uint8_t reg, const char* data, uint8_t numBytes) {
    commodule_writeRegisterBurst(reg, (uint8_t*) data, numBytes);
}
