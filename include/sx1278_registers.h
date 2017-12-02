#ifndef SX1278_REGISTERS_H_INCLUDED
#define SX1278_REGISTERS_H_INCLUDED

//SX1278 register map
#define SX1278_REG_FIFO                               0x00
#define SX1278_REG_OP_MODE                            0x01
#define SX1278_REG_FRF_MSB                            0x06
#define SX1278_REG_FRF_MID                            0x07
#define SX1278_REG_FRF_LSB                            0x08
#define SX1278_REG_PA_CONFIG                          0x09
#define SX1278_REG_PA_RAMP                            0x0A
#define SX1278_REG_OCP                                0x0B
#define SX1278_REG_LNA                                0x0C
#define SX1278_REG_FIFO_ADDR_PTR                      0x0D
#define SX1278_REG_FIFO_TX_BASE_ADDR                  0x0E
#define SX1278_REG_FIFO_RX_BASE_ADDR                  0x0F
#define SX1278_REG_FIFO_RX_CURRENT_ADDR               0x10
#define SX1278_REG_IRQ_FLAGS_MASK                     0x11
#define SX1278_REG_IRQ_FLAGS                          0x12
#define SX1278_REG_RX_NB_BYTES                        0x13
#define SX1278_REG_RX_HEADER_CNT_VALUE_MSB            0x14
#define SX1278_REG_RX_HEADER_CNT_VALUE_LSB            0x15
#define SX1278_REG_RX_PACKET_CNT_VALUE_MSB            0x16
#define SX1278_REG_RX_PACKET_CNT_VALUE_LSB            0x17
#define SX1278_REG_MODEM_STAT                         0x18
#define SX1278_REG_PKT_SNR_VALUE                      0x19
#define SX1278_REG_PKT_RSSI_VALUE                     0x1A
#define SX1278_REG_RSSI_VALUE                         0x1B
#define SX1278_REG_HOP_CHANNEL                        0x1C
#define SX1278_REG_MODEM_CONFIG_1                     0x1D
#define SX1278_REG_MODEM_CONFIG_2                     0x1E
#define SX1278_REG_SYMB_TIMEOUT_LSB                   0x1F
#define SX1278_REG_PREAMBLE_MSB                       0x20
#define SX1278_REG_PREAMBLE_LSB                       0x21
#define SX1278_REG_PAYLOAD_LENGTH                     0x22
#define SX1278_REG_MAX_PAYLOAD_LENGTH                 0x23
#define SX1278_REG_HOP_PERIOD                         0x24
#define SX1278_REG_FIFO_RX_BYTE_ADDR                  0x25
#define SX1278_REG_MODEM_CONFIG_3                     0x26
#define SX1278_REG_FEI_MSB                            0x28
#define SX1278_REG_FEI_MID                            0x29
#define SX1278_REG_FEI_LSB                            0x2A
#define SX1278_REG_RSSI_WIDEBAND                      0x2C
#define SX1278_REG_DETECT_OPTIMIZE                    0x31
#define SX1278_REG_INVERT_IQ                          0x33
#define SX1278_REG_DETECTION_THRESHOLD                0x37
#define SX1278_REG_SYNC_WORD                          0x39
#define SX1278_REG_DIO_MAPPING_1                      0x40
#define SX1278_REG_DIO_MAPPING_2                      0x41
#define SX1278_REG_VERSION                            0x42
#define SX1278_REG_TCXO                               0x4B
#define SX1278_REG_PA_DAC                             0x4D
#define SX1278_REG_FORMER_TEMP                        0x5D
#define SX1278_REG_AGC_REF                            0x61
#define SX1278_REG_AGC_THRESH_1                       0x62
#define SX1278_REG_AGC_THRESH_2                       0x63
#define SX1278_REG_AGC_THRESH_3                       0x64
#define SX1278_REG_PLL                                0x70

//SX1278 LoRa modem settings
//SX1278_REG_OP_MODE                                                  MSB   LSB   DESCRIPTION
#define SX1278_FSK_OOK                                0x00  //  7     7     FSK/OOK mode
#define SX1278_LORA                                   0x80  //  7     7     LoRa mode
#define SX1278_ACCESS_SHARED_REG_OFF                  0x00  //  6     6     access LoRa registers (0x0D:0x3F) in LoRa mode
#define SX1278_ACCESS_SHARED_REG_ON                   0x40  //  6     6     access FSK registers (0x0D:0x3F) in LoRa mode
#define SX1278_HIGH_FREQ                              0x00  //  3     3     access HF test registers
#define SX1278_LOW_FREQ                               0x08  //  3     3     access LF test registers
#define SX1278_SLEEP                                  0x00  //  2     0     sleep
#define SX1278_STANDBY                                0x01  //  2     0     standby
#define SX1278_FSTX                                   0x02  //  2     0     frequency synthesis TX
#define SX1278_TX                                     0x03  //  2     0     transmit
#define SX1278_FSRX                                   0x04  //  2     0     frequency synthesis RX
#define SX1278_RXCONTINUOUS                           0x05  //  2     0     receive continuous
#define SX1278_RXSINGLE                               0x06  //  2     0     receive single
#define SX1278_CAD                                    0x07  //  2     0     channel activity detection

//SX1278_REG_FRF_MSB + REG_FRF_MID + REG_FRF_LSB
#define SX1278_FRF_MSB                                0x6C        //  7     0     carrier frequency setting: f_RF = (F(XOSC) * FRF)/2^19
#define SX1278_FRF_MID                                0x80        //  7     0         where F(XOSC) = 32 MHz
#define SX1278_FRF_LSB                                0x00        //  7     0               FRF = 3 byte value of FRF registers

//SX1278_REG_PA_CONFIG
#define SX1278_PA_SELECT_RFO                          0x00  //  7     7     RFO pin output, power limited to +14 dBm
#define SX1278_PA_SELECT_BOOST                        0x80  //  7     7     PA_BOOST pin output, power limited to +20 dBm
#define SX1278_MAX_POWER                              0x70  //  6     4     max power: P_max = 10.8 + 0.6*MAX_POWER [dBm]; P_max(MAX_POWER = 0b111) = 15 dBm
#define SX1278_OUTPUT_POWER                           0x0F  //  3     0     output power: P_out = 17 - (15 - OUTPUT_POWER) [dBm] for PA_SELECT_BOOST

//SX1278_REG_OCP
#define SX1278_OCP_OFF                                0x00  //  5     5     PA overload current protection disabled
#define SX1278_OCP_ON                                 0x20  //  5     5     PA overload current protection enabled
#define SX1278_OCP_TRIM                               0x0B  //  4     0     OCP current: I_max(OCP_TRIM = 0b1011) = 100 mA

//SX1278_REG_LNA
#define SX1278_LNA_GAIN_0                             0x00  //  7     5     LNA gain setting:   not used
#define SX1278_LNA_GAIN_1                             0x20  //  7     5                         max gain
#define SX1278_LNA_GAIN_2                             0x40  //  7     5                         .
#define SX1278_LNA_GAIN_3                             0x60  //  7     5                         .
#define SX1278_LNA_GAIN_4                             0x80  //  7     5                         .
#define SX1278_LNA_GAIN_5                             0xA0  //  7     5                         .
#define SX1278_LNA_GAIN_6                             0xC0  //  7     5                         min gain
#define SX1278_LNA_GAIN_7                             0xE0  //  7     5                         not used
#define SX1278_LNA_BOOST_LF_OFF                       0x00  //  4     3     default LNA current
#define SX1278_LNA_BOOST_HF_OFF                       0x00  //  1     0     default LNA current
#define SX1278_LNA_BOOST_HF_ON                        0x03  //  1     0     150% LNA current

//SX1278_REG_MODEM_CONFIG_1
#define SX1278_BW_7_80_KHZ                            0x00  //  7     4     bandwidth:  7.80 kHz
#define SX1278_BW_10_40_KHZ                           0x10  //  7     4                 10.40 kHz
#define SX1278_BW_15_60_KHZ                           0x20  //  7     4                 15.60 kHz
#define SX1278_BW_20_80_KHZ                           0x30  //  7     4                 20.80 kHz
#define SX1278_BW_31_25_KHZ                           0x40  //  7     4                 31.25 kHz
#define SX1278_BW_41_70_KHZ                           0x50  //  7     4                 41.70 kHz
#define SX1278_BW_62_50_KHZ                           0x60  //  7     4                 62.50 kHz
#define SX1278_BW_125_00_KHZ                          0x70  //  7     4                 125.00 kHz
#define SX1278_BW_250_00_KHZ                          0x80  //  7     4                 250.00 kHz
#define SX1278_BW_500_00_KHZ                          0x90  //  7     4                 500.00 kHz
#define SX1278_CR_4_5                                 0x02  //  3     1     error coding rate:  4/5
#define SX1278_CR_4_6                                 0x04  //  3     1                         4/6
#define SX1278_CR_4_7                                 0x06  //  3     1                         4/7
#define SX1278_CR_4_8                                 0x08  //  3     1                         4/8
#define SX1278_HEADER_EXPL_MODE                       0x00  //  0     0     explicit header mode
#define SX1278_HEADER_IMPL_MODE                       0x01  //  0     0     implicit header mode

//SX1278_REG_MODEM_CONFIG_2
#define SX1278_SF_6                                   0x60  //  7     4     spreading factor:   64 chips/bit
#define SX1278_SF_7                                   0x70  //  7     4                         128 chips/bit
#define SX1278_SF_8                                   0x80  //  7     4                         256 chips/bit
#define SX1278_SF_9                                   0x90  //  7     4                         512 chips/bit
#define SX1278_SF_10                                  0xA0  //  7     4                         1024 chips/bit
#define SX1278_SF_11                                  0xB0  //  7     4                         2048 chips/bit
#define SX1278_SF_12                                  0xC0  //  7     4                         4096 chips/bit
#define SX1278_TX_MODE_SINGLE                         0x00  //  3     3     single TX
#define SX1278_TX_MODE_CONT                           0x08  //  3     3     continuous TX
#define SX1278_RX_CRC_MODE_OFF                        0x00  //  2     2     CRC disabled
#define SX1278_RX_CRC_MODE_ON                         0x04  //  2     2     CRC enabled
#define SX1278_RX_TIMEOUT_MSB                         0x00  //  1     0

//SX1278_REG_SYMB_TIMEOUT_LSB
#define SX1278_RX_TIMEOUT_LSB                         0x64  //  7     0     10 bit RX operation timeout

//SX1278_REG_PREAMBLE_MSB + REG_PREAMBLE_LSB
#define SX1278_PREAMBLE_LENGTH_MSB                    0x00  //  7     0     2 byte preamble length setting: l_P = PREAMBLE_LENGTH + 4.25
#define SX1278_PREAMBLE_LENGTH_LSB                    0x08  //  7     0         where l_p = preamble length

//SX1278_REG_MODEM_CONFIG_3
#define SX1278_LOW_DATA_RATE_OPT_OFF                  0x00  //  3     3     low data rate optimization disabled
#define SX1278_LOW_DATA_RATE_OPT_ON                   0x08  //  3     3     low data rate optimization enabled
#define SX1278_AGC_AUTO_OFF                           0x00  //  2     2     LNA gain set by REG_LNA
#define SX1278_AGC_AUTO_ON                            0x04  //  2     2     LNA gain set by internal AGC loop

//SX1278_REG_DETECT_OPTIMIZE
#define SX1278_DETECT_OPTIMIZE_SF_6                   0x05  //  2     0     SF6 detection optimization
#define SX1278_DETECT_OPTIMIZE_SF_7_12                0x03  //  2     0     SF7 to SF12 detection optimization

//SX1278_REG_DETECTION_THRESHOLD
#define SX1278_DETECTION_THRESHOLD_SF_6               0x0C  //  7     0     SF6 detection threshold
#define SX1278_DETECTION_THRESHOLD_SF_7_12            0x0A  //  7     0     SF7 to SF12 detection threshold

//SX1278_REG_PA_DAC
#define SX1278_PA_BOOST_OFF                           0x04  //  2     0     PA_BOOST disabled
#define SX1278_PA_BOOST_ON                            0x07  //  2     0     +20 dBm on PA_BOOST when OUTPUT_POWER = 0b1111

//SX1278_REG_HOP_PERIOD
#define SX1278_HOP_PERIOD_OFF                         0x00  //  7     0     number of periods between frequency hops; 0 = disabled
#define SX1278_HOP_PERIOD_MAX                         0xFF  //  7     0

//SX1278_REG_DIO_MAPPING_1
#define SX1278_DIO0_RX_DONE                           0x00  //  7     6
#define SX1278_DIO0_TX_DONE                           0x40  //  7     6
#define SX1278_DIO0_CAD_DONE                          0x80  //  7     6
#define SX1278_DIO1_RX_TIMEOUT                        0x00  //  5     4
#define SX1278_DIO1_FHSS_CHANGE_CHANNEL               0x10  //  5     4
#define SX1278_DIO1_CAD_DETECTED                      0x20  //  5     4

//SX1278_REG_IRQ_FLAGS
#define SX1278_CLEAR_IRQ_FLAG_RX_TIMEOUT              0x80  //  7     7     timeout
#define SX1278_CLEAR_IRQ_FLAG_RX_DONE                 0x40  //  6     6     packet reception complete
#define SX1278_CLEAR_IRQ_FLAG_PAYLOAD_CRC_ERROR       0x20  //  5     5     payload CRC error
#define SX1278_CLEAR_IRQ_FLAG_VALID_HEADER            0x10  //  4     4     valid header received
#define SX1278_CLEAR_IRQ_FLAG_TX_DONE                 0x08  //  3     3     payload transmission complete
#define SX1278_CLEAR_IRQ_FLAG_CAD_DONE                0x04  //  2     2     CAD complete
#define SX1278_CLEAR_IRQ_FLAG_FHSS_CHANGE_CHANNEL     0x02  //  1     1     FHSS change channel
#define SX1278_CLEAR_IRQ_FLAG_CAD_DETECTED            0x01  //  0     0     valid LoRa signal detected during CAD operation

//SX1278_REG_IRQ_FLAGS_MASK
#define SX1278_MASK_IRQ_FLAG_RX_TIMEOUT               0x7F  //  7     7     timeout
#define SX1278_MASK_IRQ_FLAG_RX_DONE                  0xBF  //  6     6     packet reception complete
#define SX1278_MASK_IRQ_FLAG_PAYLOAD_CRC_ERROR        0xDF  //  5     5     payload CRC error
#define SX1278_MASK_IRQ_FLAG_VALID_HEADER             0xEF  //  4     4     valid header received
#define SX1278_MASK_IRQ_FLAG_TX_DONE                  0xF7  //  3     3     payload transmission complete
#define SX1278_MASK_IRQ_FLAG_CAD_DONE                 0xFB  //  2     2     CAD complete
#define SX1278_MASK_IRQ_FLAG_FHSS_CHANGE_CHANNEL      0xFD  //  1     1     FHSS change channel
#define SX1278_MASK_IRQ_FLAG_CAD_DETECTED             0xFE  //  0     0     valid LoRa signal detected during CAD operation

//SX1278_REG_FIFO_TX_BASE_ADDR
#define SX1278_FIFO_TX_BASE_ADDR_MAX                  0x00  //  7     0     allocate the entire FIFO buffer for TX only

//SX1278_REG_FIFO_RX_BASE_ADDR
#define SX1278_FIFO_RX_BASE_ADDR_MAX                  0x00  //  7     0     allocate the entire FIFO buffer for RX only

#endif
