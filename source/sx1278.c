#include "sx1278_registers.h"
#include "sx1278.h"
#include "commodule.h"
#include "delay.h"

uint8_t _bw, _sf, _cr;

static hld_pin_ro_t* _pin_dio0;
static hld_pin_ro_t* _pin_dio1;
static hld_pin_wo_t* _pin_txen;
static hld_pin_wo_t* _pin_rxen;
static hld_pin_wo_t* _pin_rst;
static hld_spi_t* _spi;

uint8_t sx1278_init (
        hld_spi_t* spi,
        hld_pin_ro_t* pin_dio0,
        hld_pin_ro_t * pin_dio1,
        hld_pin_wo_t * pin_txen,
        hld_pin_wo_t * pin_rxen,
        hld_pin_wo_t * pin_rst,
        bandwidth bw,
        spreadingfactor sf,
        codingrate cr
        )
{
    _pin_dio0 = pin_dio0;
    _pin_dio1 = pin_dio1;
    _pin_txen = pin_txen;
    _pin_rxen = pin_rxen;
    _pin_rst = pin_rst;
    _spi = spi;

    _pin_txen->write(0);
    _pin_rxen->write(0);
    _pin_rst->write(0);
    delay_ms(1);
    _pin_rst->write(1);
    delay_ms(20);


  switch(bw) {
    case BW_7_80_KHZ:
      _bw = SX1278_BW_7_80_KHZ;
      break;
    case BW_10_40_KHZ:
      _bw = SX1278_BW_10_40_KHZ;
      break;
    case BW_15_60_KHZ:
      _bw = SX1278_BW_15_60_KHZ;
      break;
    case BW_20_80_KHZ:
      _bw = SX1278_BW_20_80_KHZ;
      break;
    case BW_31_25_KHZ:
      _bw = SX1278_BW_31_25_KHZ;
      break;
      case BW_41_70_KHZ:
      _bw = SX1278_BW_41_70_KHZ;
      break;
    case BW_62_50_KHZ:
      _bw = SX1278_BW_62_50_KHZ;
      break;
    case BW_125_00_KHZ:
      _bw = SX1278_BW_125_00_KHZ;
      break;
    case BW_250_00_KHZ:
      _bw = SX1278_BW_250_00_KHZ;
      break;
    case BW_500_00_KHZ:
      _bw = SX1278_BW_500_00_KHZ;
      break;
    default:
      _bw = SX1278_BW_250_00_KHZ;
      break;
  }

  switch(sf) {
    case SF_6:
      _sf = SX1278_SF_6;
      break;
    case SF_7:
      _sf = SX1278_SF_7;
      break;
    case SF_8:
      _sf = SX1278_SF_8;
      break;
    case SF_9:
      _sf = SX1278_SF_9;
      break;
    case SF_10:
      _sf = SX1278_SF_10;
      break;
    case SF_11:
      _sf = SX1278_SF_11;
      break;
    case SF_12:
      _sf = SX1278_SF_12;
      break;
    default:
      _sf = SX1278_SF_12;
      break;
  }

  switch(cr) {
    case CR_4_5:
      _cr = SX1278_CR_4_5;
      break;
    case CR_4_6:
      _cr = SX1278_CR_4_6;
      break;
    case CR_4_7:
      _cr = SX1278_CR_4_7;
      break;
    case CR_4_8:
      _cr = SX1278_CR_4_8;
      break;
    default:
      _cr = SX1278_CR_4_5;
      break;
  }
  
  commodule_init(_spi);
  return(sx1278_config(_bw, _sf, _cr));
}

uint8_t sx1278_config(uint8_t bw, uint8_t sf, uint8_t cr) {
  uint8_t status = ERR_NONE;

  //check the supplied bw, cr and sf values
  if((bw != SX1278_BW_7_80_KHZ) &&
     (bw != SX1278_BW_10_40_KHZ) &&
     (bw != SX1278_BW_15_60_KHZ) &&
     (bw != SX1278_BW_20_80_KHZ) &&
     (bw != SX1278_BW_31_25_KHZ) &&
     (bw != SX1278_BW_41_70_KHZ) &&
     (bw != SX1278_BW_62_50_KHZ) &&
     (bw != SX1278_BW_125_00_KHZ) &&
     (bw != SX1278_BW_250_00_KHZ) &&
     (bw != SX1278_BW_500_00_KHZ)) {
       return(ERR_INVALID_BANDWIDTH);
  }

  if((sf != SX1278_SF_6) &&
     (sf != SX1278_SF_7) &&
     (sf != SX1278_SF_8) &&
     (sf != SX1278_SF_9) &&
     (sf != SX1278_SF_10) &&
     (sf != SX1278_SF_11) &&
     (sf != SX1278_SF_12)) {
       return(ERR_INVALID_SPREADING_FACTOR);
  }

  if((cr != SX1278_CR_4_5) &&
     (cr != SX1278_CR_4_6) &&
     (cr != SX1278_CR_4_7) &&
     (cr != SX1278_CR_4_8)) {
       return(ERR_INVALID_CODING_RATE);
  }

  // set mode to SLEEP
  status = sx1278_setmode(SX1278_SLEEP);
  if(status != ERR_NONE) {
    return(status);
  }

  // set LoRa mode
  status = commodule_setRegValue(SX1278_REG_OP_MODE, SX1278_LORA, 7, 7);
  if(status != ERR_NONE) {
    return(status);
  }

  // set carrier frequency
  status = commodule_setRegValue(SX1278_REG_FRF_MSB, SX1278_FRF_MSB, 7, 0);
  status = commodule_setRegValue(SX1278_REG_FRF_MID, SX1278_FRF_MID, 7, 0);
  status = commodule_setRegValue(SX1278_REG_FRF_LSB, SX1278_FRF_LSB, 7, 0);
  if(status != ERR_NONE) {
    return(status);
  }

  // output power configuration
  status = commodule_setRegValue(SX1278_REG_PA_CONFIG, SX1278_PA_SELECT_BOOST | SX1278_MAX_POWER | SX1278_OUTPUT_POWER, 7, 0);
  status = commodule_setRegValue(SX1278_REG_OCP, SX1278_OCP_ON | SX1278_OCP_TRIM, 5, 0);
  status = commodule_setRegValue(SX1278_REG_LNA, SX1278_LNA_GAIN_1 | SX1278_LNA_BOOST_HF_ON, 7, 0);
  status = commodule_setRegValue(SX1278_REG_PA_DAC, SX1278_PA_BOOST_ON, 2, 0);
  if(status != ERR_NONE) {
    return(status);
  }

  // turn off frequency hopping
  status = commodule_setRegValue(SX1278_REG_HOP_PERIOD, SX1278_HOP_PERIOD_OFF, 7, 0);
  if(status != ERR_NONE) {
    return(status);
  }

  // basic setting (bw, cr, sf, header mode and CRC)
  if(sf == SX1278_SF_6) {
    status = commodule_setRegValue(SX1278_REG_MODEM_CONFIG_2, SX1278_SF_6 | SX1278_TX_MODE_SINGLE | SX1278_RX_CRC_MODE_OFF, 7, 2);
    status = commodule_setRegValue(SX1278_REG_MODEM_CONFIG_1, bw | cr | SX1278_HEADER_IMPL_MODE, 7, 0);
    status = commodule_setRegValue(SX1278_REG_DETECT_OPTIMIZE, SX1278_DETECT_OPTIMIZE_SF_6, 2, 0);
    status = commodule_setRegValue(SX1278_REG_DETECTION_THRESHOLD, SX1278_DETECTION_THRESHOLD_SF_6, 7, 0);
  } else {
    status = commodule_setRegValue(SX1278_REG_MODEM_CONFIG_2, sf | SX1278_TX_MODE_SINGLE | SX1278_RX_CRC_MODE_ON, 7, 2);
    status = commodule_setRegValue(SX1278_REG_MODEM_CONFIG_1, bw | cr | SX1278_HEADER_EXPL_MODE, 7, 0);
    status = commodule_setRegValue(SX1278_REG_DETECT_OPTIMIZE, SX1278_DETECT_OPTIMIZE_SF_7_12, 2, 0);
    status = commodule_setRegValue(SX1278_REG_DETECTION_THRESHOLD, SX1278_DETECTION_THRESHOLD_SF_7_12, 7, 0);
  }

  if(status != ERR_NONE) {
    return(status);
  }

  // set default preamble length
  status = commodule_setRegValue(SX1278_REG_PREAMBLE_MSB, SX1278_PREAMBLE_LENGTH_MSB, 7, 0);
  status = commodule_setRegValue(SX1278_REG_PREAMBLE_LSB, SX1278_PREAMBLE_LENGTH_LSB, 7, 0);
  if(status != ERR_NONE) {
    return(status);
  }

  // set mode to STANDBY
  status = sx1278_setmode(SX1278_STANDBY);
  if(status != ERR_NONE) {
    return(status);
  }

  return(ERR_NONE);
}

uint8_t sx1278_tx(char* data, uint8_t length) {
  sx1278_setmode(SX1278_STANDBY);
  _pin_txen->write(0);
  _pin_rxen->write(0);
  delay_ms(1);
  _pin_txen->write(1);
  delay_ms(1);

  commodule_setRegValue(SX1278_REG_DIO_MAPPING_1, SX1278_DIO0_TX_DONE, 7, 6);
  sx1278_clearirqflags();

  commodule_setRegValue(SX1278_REG_PAYLOAD_LENGTH, length, 7, 0);
  commodule_setRegValue(SX1278_REG_FIFO_TX_BASE_ADDR, SX1278_FIFO_TX_BASE_ADDR_MAX, 7, 0);
  commodule_setRegValue(SX1278_REG_FIFO_ADDR_PTR, SX1278_FIFO_TX_BASE_ADDR_MAX, 7, 0);

  commodule_writeRegisterBurstStr(SX1278_REG_FIFO, data, length);

  sx1278_setmode(SX1278_TX);

  while(!_pin_dio0->read()) {
  }

  sx1278_clearirqflags();

  return(ERR_NONE);
}

uint8_t sx1278_rxSingle(char* data, uint8_t* length) {
  sx1278_setmode(SX1278_STANDBY);

  _pin_txen->write(0);
  _pin_rxen->write(0);
  delay_ms(1);
  _pin_rxen->write(1);
  delay_ms(1);

  commodule_setRegValue(SX1278_REG_DIO_MAPPING_1, SX1278_DIO0_RX_DONE | SX1278_DIO1_RX_TIMEOUT, 7, 4);
  sx1278_clearirqflags();

  commodule_setRegValue(SX1278_REG_FIFO_RX_BASE_ADDR, SX1278_FIFO_RX_BASE_ADDR_MAX, 7 , 0);
  commodule_setRegValue(SX1278_REG_FIFO_ADDR_PTR, SX1278_FIFO_RX_BASE_ADDR_MAX, 7 , 0);

  sx1278_setmode(SX1278_RXSINGLE);

  while(!_pin_dio0->read()) {
    if(_pin_dio1->read()) {
      sx1278_clearirqflags();
      return(ERR_RX_TIMEOUT);
    }
  }

  if(commodule_getRegValue(SX1278_REG_IRQ_FLAGS, 5, 5) == SX1278_CLEAR_IRQ_FLAG_PAYLOAD_CRC_ERROR) {
    return(ERR_CRC_MISMATCH);
  }

  uint8_t headerMode = commodule_getRegValue(SX1278_REG_MODEM_CONFIG_1, 0, 0);
  if(headerMode == SX1278_HEADER_EXPL_MODE) {
    *length = commodule_getRegValue(SX1278_REG_RX_NB_BYTES, 7 , 0);
  }

  commodule_readRegisterBurstStr(SX1278_REG_FIFO, *length, data);

  sx1278_clearirqflags();

  return(ERR_NONE);
}

uint8_t sx1278_setmode(uint8_t mode) {
  commodule_setRegValue(SX1278_REG_OP_MODE, mode, 2, 0);
  return(ERR_NONE);
}


int8_t sx1278_getLastPacketRSSI(void) {
  return(-164 + commodule_getRegValue(SX1278_REG_PKT_RSSI_VALUE, 7 , 0));
}

void sx1278_clearirqflags(void) {
  commodule_writeRegister(SX1278_REG_IRQ_FLAGS, 0xFF);
}
