#ifndef SX1278_H_INCLUDED
#define SX1278_H_INCLUDED

#include "hld_api.h"

enum Bandwidth {BW_7_80_KHZ, BW_10_40_KHZ, BW_15_60_KHZ, BW_20_80_KHZ, BW_31_25_KHZ, BW_41_70_KHZ, BW_62_50_KHZ, BW_125_00_KHZ, BW_250_00_KHZ, BW_500_00_KHZ};
enum SpreadingFactor {SF_6, SF_7, SF_8, SF_9, SF_10, SF_11, SF_12};
enum CodingRate {CR_4_5, CR_4_6, CR_4_7, CR_4_8};

typedef enum Bandwidth bandwidth;
typedef enum SpreadingFactor spreadingfactor;
typedef enum CodingRate codingrate;

uint8_t sx1278_init(hld_spi_t* spi, hld_pin_ro_t* pin0, hld_pin_ro_t* pin1, bandwidth bw, spreadingfactor sf, codingrate cr);
uint8_t sx1278_tx(char* data, uint8_t length);
uint8_t sx1278_rxsingle(char* data, uint8_t* length);
uint8_t sx1278_setmode(uint8_t mode);
uint8_t sx1278_config(uint8_t bw, uint8_t sf, uint8_t cr);
int8_t sx1278_getlastpacketrssi(void);
void sx1278_clearirqflags(void);

#endif
