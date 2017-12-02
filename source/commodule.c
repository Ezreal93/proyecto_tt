#include "commodule.h"
#include <string.h>

static hld_spi_t* _spi;

uint8_t module_init(hld_spi_t* spi) {
    _spi = spi;
    return ERR_NONE;
}

uint8_t commodule_getRegValue(uint8_t reg, uint8_t msb, uint8_t lsb) {
    uint8_t rawValue = commodule_readRegister(reg);
    uint8_t maskedValue = rawValue & ((0xFF << lsb) & (0xFF >> (7 - msb)));
    return(maskedValue);
}

uint8_t commodule_readRegister(uint8_t reg) {
    uint8_t inByte;
    _spi->xfer(&reg, 1, &inByte, 1);
    return(inByte);
}

uint8_t commodule_readRegisterBurst(uint8_t reg, uint8_t numBytes, uint8_t* inBytes) {
    _spi->xfer(&reg, 1, inBytes, numBytes);
    return(ERR_NONE);
}


uint8_t commodule_readRegisterBurstStr(uint8_t reg, uint8_t numBytes, char* inBytes) {
    return commodule_readRegisterBurst(reg, numBytes, (uint8_t*) inBytes);
}

uint8_t commodule_setRegValue(uint8_t reg, uint8_t value, uint8_t msb, uint8_t lsb) {
    uint8_t currentValue = commodule_readRegister(reg);
    uint8_t newValue = currentValue & ((0xFF << (msb + 1)) & (0xFF >> (8 - lsb)));
    commodule_writeRegister(reg, newValue | value);
    return(ERR_NONE);
}

void commodule_writeRegister(uint8_t reg, uint8_t data) {
    uint8_t tempBuffer[2] = {reg, data};
    _spi->xfer(tempBuffer, 2, NULL, 0);
}

void commodule_writeRegisterBurst(uint8_t reg, uint8_t* data, uint8_t numBytes) {
    uint8_t tempBuffer[numBytes + 1];
    tempBuffer[0] = reg;
    memcpy(tempBuffer, data, numBytes);

    _spi->xfer(tempBuffer, numBytes + 1, NULL, 0);
}

void commodule_writeRegisterBurstStr(uint8_t reg, const char* data, uint8_t numBytes) {
    commodule_writeRegisterBurst(reg, (uint8_t*) data, numBytes);
}
