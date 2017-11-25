#ifndef BMP280_H__
#define BMP280_H__

#include <stdint.h>

/************************************************/
/**\name	BUS READ AND WRITE FUNCTION POINTERS */
/***********************************************/
typedef int8_t (*bus_write_t)(uint8_t, uint8_t, uint8_t *, uint8_t);

typedef	int8_t (*bus_read_t)(uint8_t, uint8_t, uint8_t *, uint8_t);

#define BMP280_MDELAY_DATA_TYPE uint32_t

/**************************************************************/
/**\name	STRUCTURE DEFINITIONS                         */
/**************************************************************/
/*!
 * @brief This structure holds all device specific calibration parameters
 */
typedef struct {
	uint16_t dig_T1;/**<calibration T1 data*/
	int16_t dig_T2;/**<calibration T2 data*/
	int16_t dig_T3;/**<calibration T3 data*/
	uint16_t dig_P1;/**<calibration P1 data*/
	int16_t dig_P2;/**<calibration P2 data*/
	int16_t dig_P3;/**<calibration P3 data*/
	int16_t dig_P4;/**<calibration P4 data*/
	int16_t dig_P5;/**<calibration P5 data*/
	int16_t dig_P6;/**<calibration P6 data*/
	int16_t dig_P7;/**<calibration P7 data*/
	int16_t dig_P8;/**<calibration P8 data*/
	int16_t dig_P9;/**<calibration P9 data*/

	int32_t t_fine;/**<calibration t_fine data*/
} bmp280_calib_param_t;
/*!
 * @brief This structure holds BMP280 initialization parameters
 */
typedef struct {
	bmp280_calib_param_t calib_param;/**<calibration data*/

	uint8_t chip_id;/**< chip id of the sensor*/
	uint8_t dev_addr;/**< device address of the sensor*/

	uint8_t oversamp_temperature;/**< temperature over sampling*/
	uint8_t oversamp_pressure;/**< pressure over sampling*/

	bus_write_t bus_write;/**< bus write function pointer*/
	bus_read_t  bus_read;/**< bus read function pointer*/
	void (*delay_msec)(BMP280_MDELAY_DATA_TYPE);/**< delay function pointer*/
} bmp280_t;

/**************************************************************/
/**\name	FUNCTION DECLARATIONS                         */
/**************************************************************/
/**************************************************************/
/**\name	FUNCTION FOR  INTIALIZATION                       */
/**************************************************************/
/*!
 *	@brief This function is used for initialize
 *	the bus read and bus write functions
 *  and assign the chip id and I2C address of the BMP280 sensor
 *	chip id is read in the register 0xD0 bit from 0 to 7
 *
 *	@param *bmp280 structure pointer.
 *
 *	@note While changing the parameter of the p_bmp280
 *	@note consider the following point:
 *	Changing the reference value of the parameter
 *	will changes the local copy or local reference
 *	make sure your changes will not
 *	affect the reference value of the parameter
 *	(Better case don't change the reference value of the parameter)
 *
 *
 *
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
int8_t bmp280_init(bmp280_t *bmp280);
/**************************************************************/
/**\name	FUNCTION FOR READ UNCOMPENSATED TEMPERATURE     */
/**************************************************************/
/*!
 *	@brief This API is used to read uncompensated temperature
 *	in the registers 0xFA, 0xFB and 0xFC
 *	@note 0xFA -> MSB -> bit from 0 to 7
 *	@note 0xFB -> LSB -> bit from 0 to 7
 *	@note 0xFC -> LSB -> bit from 4 to 7
 *
 *	@param v_uncomp_temperature_s32 : The uncompensated temperature.
 *
 *
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
int8_t bmp280_read_uncomp_temperature(
		int32_t *v_uncomp_temperature_s32);
/**************************************************************/
/**\name	FUNCTION FOR READ TRUE TEMPERATURE int32_t OUTPUT    */
/**************************************************************/
/*!
 *	@brief Reads actual temperature
 *	from uncompensated temperature
 *	@note Returns the value in 0.01 degree Centigrade
 *	@note Output value of "5123" equals 51.23 DegC.
 *
 *
 *
 *  @param v_uncomp_temperature_s32 : value of uncompensated temperature
 *
 *
 *
 *  @return Actual temperature output as int32_t
 *
*/
int32_t bmp280_compensate_temperature_int32(int32_t v_uncomp_temperature_s32);
/**************************************************************/
/**\name	FUNCTION FOR READ UNCOMPENSATED PRESSURE     */
/**************************************************************/
/*!
 *	@brief This API is used to read uncompensated pressure.
 *	in the registers 0xF7, 0xF8 and 0xF9
 *	@note 0xF7 -> MSB -> bit from 0 to 7
 *	@note 0xF8 -> LSB -> bit from 0 to 7
 *	@note 0xF9 -> LSB -> bit from 4 to 7
 *
 *
 *
 *	@param v_uncomp_pressure_s32 : The value of uncompensated pressure
 *
 *
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
int8_t bmp280_read_uncomp_pressure(
		int32_t *v_uncomp_pressure_s32);
/**************************************************************/
/**\name	FUNCTION FOR READ TRUE PRESSURE int32_t OUTPUT    */
/**************************************************************/
/*!
 *	@brief Reads actual pressure from uncompensated pressure
 *	and returns the value in Pascal(Pa)
 *	@note Output value of "96386" equals 96386 Pa =
 *	963.86 hPa = 963.86 millibar
 *
 *
 *
 *
 *  @param  v_uncomp_pressure_s32: value of uncompensated pressure
 *
 *
 *
 *  @return Returns the Actual pressure out put as int32_t
 *
*/
uint32_t bmp280_compensate_pressure_int32(int32_t v_uncomp_pressure_s32);
/**************************************************************/
/**\name	FUNCTION FOR READ UNCOMPENSATED TEMPERATURE AND PRESSURE  */
/**************************************************************/
/*!
 * @brief reads uncompensated pressure and temperature
 *
 *
 * @param  v_uncomp_pressure_s32: The value of uncompensated pressure.
 * @param  v_uncomp_temperature_s32: The value of uncompensated temperature.
 *
 *  @return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
*/
int8_t bmp280_read_uncomp_pressure_temperature(
		int32_t *v_uncomp_pressure_s32, int32_t *v_uncomp_temperature_s32);
/**************************************************************/
/**\name	FUNCTION FOR READ TRUE TEMPERATURE AND PRESSURE    */
/**************************************************************/
/*!
 * @brief This API reads the true pressure and temperature
 *
 *
 *  @param  v_pressure_u32 : The value of compensated pressure.
 *  @param  v_temperature_s32 : The value of compensated temperature.
 *
 *
 *  @return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
int8_t bmp280_read_pressure_temperature(
		uint32_t *v_pressure_u32, int32_t *v_pressure_s32);
/**************************************************************/
/**\name	FUNCTION FOR READ CALIBRATION DATA    */
/**************************************************************/
/*!
 *	@brief This API is used to
 *	calibration parameters used for calculation in the registers
 *
 *  parameter | Register address |   bit
 *------------|------------------|----------------
 *	dig_T1    |  0x88 and 0x89   | from 0 : 7 to 8: 15
 *	dig_T2    |  0x8A and 0x8B   | from 0 : 7 to 8: 15
 *	dig_T3    |  0x8C and 0x8D   | from 0 : 7 to 8: 15
 *	dig_P1    |  0x8E and 0x8F   | from 0 : 7 to 8: 15
 *	dig_P2    |  0x90 and 0x91   | from 0 : 7 to 8: 15
 *	dig_P3    |  0x92 and 0x93   | from 0 : 7 to 8: 15
 *	dig_P4    |  0x94 and 0x95   | from 0 : 7 to 8: 15
 *	dig_P5    |  0x96 and 0x97   | from 0 : 7 to 8: 15
 *	dig_P6    |  0x98 and 0x99   | from 0 : 7 to 8: 15
 *	dig_P7    |  0x9A and 0x9B   | from 0 : 7 to 8: 15
 *	dig_P8    |  0x9C and 0x9D   | from 0 : 7 to 8: 15
 *	dig_P9    |  0x9E and 0x9F   | from 0 : 7 to 8: 15
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
int8_t bmp280_get_calib_param(void);
/**************************************************************/
/**\name	FUNCTION FOR OVERSAMPLING TEMPERATURE AND PRESSURE    */
/**************************************************************/
/*!
 *	@brief This API is used to get
 *	the temperature oversampling setting in the register 0xF4
 *	bits from 5 to 7
 *
 *        value             | Temperature oversampling
 *  ------------------------|------------------------------
 *       0x00               |  BMP280_OVERSAMP_SKIPPED
 *       0x01               |  BMP280_OVERSAMP_1X
 *       0x02               |  BMP280_OVERSAMP_2X
 *       0x03               |  BMP280_OVERSAMP_4X
 *       0x04               |  BMP280_OVERSAMP_8X
 *       0x05,0x06 and 0x07 |  BMP280_OVERSAMP_16X
 *
 *
 *  @param v_value_u8 :The value of temperature over sampling
 *
 *
 *
 *  @return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
int8_t bmp280_get_oversamp_temperature(uint8_t *v_value_u8);
/*!
 *	@brief This API is used to set
 *	the temperature oversampling setting in the register 0xF4
 *	bits from 5 to 7
 *
 *        value             | Temperature oversampling
 *  ------------------------|------------------------------
 *       0x00               |  BMP280_OVERSAMP_SKIPPED
 *       0x01               |  BMP280_OVERSAMP_1X
 *       0x02               |  BMP280_OVERSAMP_2X
 *       0x03               |  BMP280_OVERSAMP_4X
 *       0x04               |  BMP280_OVERSAMP_8X
 *       0x05,0x06 and 0x07 |  BMP280_OVERSAMP_16X
 *
 *
 *  @param v_value_u8 :The value of temperature over sampling
 *
 *
 *
 *  @return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
int8_t bmp280_set_oversamp_temperature(uint8_t v_value_u8);
/*!
 *	@brief This API is used to get
 *	the pressure oversampling setting in the register 0xF4
 *	bits from 2 to 4
 *
 *        value             | Pressure oversampling
 *  ------------------------|------------------------------
 *       0x00               |  BMP280_OVERSAMP_SKIPPED
 *       0x01               |  BMP280_OVERSAMP_1X
 *       0x02               |  BMP280_OVERSAMP_2X
 *       0x03               |  BMP280_OVERSAMP_4X
 *       0x04               |  BMP280_OVERSAMP_8X
 *       0x05,0x06 and 0x07 |  BMP280_OVERSAMP_16X
 *
 *
 *  @param  v_value_u8 : The value of pressure over sampling
 *
 *
 *
 *  @return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
int8_t bmp280_get_oversamp_pressure(uint8_t *v_value_u8);
/*!
 *	@brief This API is used to set
 *	the pressure oversampling setting in the register 0xF4
 *	bits from 2 to 4
 *
 *        value             | Pressure oversampling
 *  ------------------------|------------------------------
 *       0x00               |  BMP280_OVERSAMP_SKIPPED
 *       0x01               |  BMP280_OVERSAMP_1X
 *       0x02               |  BMP280_OVERSAMP_2X
 *       0x03               |  BMP280_OVERSAMP_4X
 *       0x04               |  BMP280_OVERSAMP_8X
 *       0x05,0x06 and 0x07 |  BMP280_OVERSAMP_16X
 *
 *
 *  @param  v_value_u8 : The value of pressure over sampling
 *
 *
 *
 *  @return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
int8_t bmp280_set_oversamp_pressure(uint8_t v_value_u8);
/**************************************************************/
/**\name	FUNCTION FOR POWER MODE    */
/**************************************************************/
/*!
 *	@brief This API used to get the
 *	Operational Mode from the sensor in the register 0xF4 bit 0 and 1
 *
 *
 *
 *	@param v_power_mode_u8 : The value of power mode value
 *  value            |   Power mode
 * ------------------|------------------
 *	0x00             | BMP280_SLEEP_MODE
 *	0x01 and 0x02    | BMP280_FORCED_MODE
 *	0x03             | BMP280_NORMAL_MODE
 *
 *  @return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
int8_t bmp280_get_power_mode(uint8_t *v_power_mode_u8);
/*!
 *	@brief This API used to set the
 *	Operational Mode from the sensor in the register 0xF4 bit 0 and 1
 *
 *
 *
 *	@param v_power_mode_u8 : The value of power mode value
 *  value            |   Power mode
 * ------------------|------------------
 *	0x00             | BMP280_SLEEP_MODE
 *	0x01 and 0x02    | BMP280_FORCED_MODE
 *	0x03             | BMP280_NORMAL_MODE
 *
 *  @return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
int8_t bmp280_set_power_mode(uint8_t v_power_mode_u8);
/**************************************************************/
/**\name	FUNCTION FOR SOFT RESET   */
/**************************************************************/
/*!
 * @brief Used to reset the sensor
 * The value 0xB6 is written to the
 * 0xE0 register the device is reset using the
 * complete power-on-reset procedure.
 * Soft reset can be easily set using bmp280_set_softreset().
 *
 * @note Usage Hint : bmp280_set_softreset()
 *
 *
 *  @return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
int8_t bmp280_set_soft_rst(void);
/**************************************************************/
/**\name	FUNCTION FOR SPI ENABLE    */
/**************************************************************/
/*!
 *	@brief This API used to get the sensor
 *	SPI mode(communication type) in the register 0xF5 bit 0
 *
 *
 *
 *	@param v_enable_disable_u8 : The spi3 enable or disable state
 *    value    | Description
 *  -----------|---------------
 *     0       | Disable
 *     1       | Enable
 *
 *
 *
 *  @return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
int8_t bmp280_get_spi3(uint8_t *v_enable_disable_u8);
/*!
 *	@brief This API used to set the sensor
 *	SPI mode(communication type) in the register 0xF5 bit 0
 *
 *
 *
 *	@param v_enable_disable_u8 : The spi3 enable or disable state
 *    value    | Description
 *  -----------|---------------
 *     0       | Disable
 *     1       | Enable
 *
 *
 *
 *  @return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
int8_t bmp280_set_spi3(uint8_t v_enable_disable_u8);
/**************************************************************/
/**\name	FUNCTION FOR IIR FILTER SETTING   */
/**************************************************************/
/*!
 *	@brief This API is used to reads filter setting
 *	in the register 0xF5 bit 3 and 4
 *
 *
 *
 *	@param v_value_u8 : The value of filter coefficient
 *	value	    |	Filter coefficient
 * -------------|-------------------------
 *	0x00        | BMP280_FILTER_COEFF_OFF
 *	0x01        | BMP280_FILTER_COEFF_2
 *	0x02        | BMP280_FILTER_COEFF_4
 *	0x03        | BMP280_FILTER_COEFF_8
 *	0x04        | BMP280_FILTER_COEFF_16
 *
 *  @return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
int8_t bmp280_get_filter(uint8_t *v_value_u8);
/*!
 *	@brief This API is used to write filter setting
 *	in the register 0xF5 bit 3 and 4
 *
 *
 *
 *	@param v_value_u8 : The value of filter coefficient
 *	value	    |	Filter coefficient
 * -------------|-------------------------
 *	0x00        | BMP280_FILTER_COEFF_OFF
 *	0x01        | BMP280_FILTER_COEFF_2
 *	0x02        | BMP280_FILTER_COEFF_4
 *	0x03        | BMP280_FILTER_COEFF_8
 *	0x04        | BMP280_FILTER_COEFF_16
 *
 *  @return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
int8_t bmp280_set_filter(uint8_t v_value_u8);
/**************************************************************/
/**\name	FUNCTION FOR STANDBY DURATION   */
/**************************************************************/
/*!
 *	@brief This API used to Read the
 *	standby duration time from the sensor in the register 0xF5 bit 5 to 7
 *
 *	@param v_standby_durn_u8 : The standby duration time value.
 *  value     |  standby duration
 * -----------|--------------------
 *    0x00    | BMP280_STANDBYTIME_1_MS
 *    0x01    | BMP280_STANDBYTIME_63_MS
 *    0x02    | BMP280_STANDBYTIME_125_MS
 *    0x03    | BMP280_STANDBYTIME_250_MS
 *    0x04    | BMP280_STANDBYTIME_500_MS
 *    0x05    | BMP280_STANDBYTIME_1000_MS
 *    0x06    | BMP280_STANDBYTIME_2000_MS
 *    0x07    | BMP280_STANDBYTIME_4000_MS
 *
 *
 *
 *  @return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
int8_t bmp280_get_standby_durn(uint8_t *v_standby_durn_u8);
/*!
 *	@brief This API used to Read the
 *	standby duration time from the sensor in the register 0xF5 bit 5 to 7
 *	@note Normal mode comprises an automated perpetual cycling between an (active)
 *	Measurement period and an (inactive) standby period.
 *	@note The standby time is determined by the contents of the register t_sb.
 *	Standby time can be set using BMP280_STANDBYTIME_125_MS.
 *
 *	@note bmp280_set_standby_durn(BMP280_STANDBYTIME_125_MS)
 *
 *
 *
 *	@param v_standby_durn_u8 : The standby duration time value.
 *  value     |  standby duration
 * -----------|--------------------
 *    0x00    | BMP280_STANDBYTIME_1_MS
 *    0x01    | BMP280_STANDBYTIME_63_MS
 *    0x02    | BMP280_STANDBYTIME_125_MS
 *    0x03    | BMP280_STANDBYTIME_250_MS
 *    0x04    | BMP280_STANDBYTIME_500_MS
 *    0x05    | BMP280_STANDBYTIME_1000_MS
 *    0x06    | BMP280_STANDBYTIME_2000_MS
 *    0x07    | BMP280_STANDBYTIME_4000_MS
 *
 *
 *
 *  @return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
int8_t bmp280_set_standby_durn(uint8_t v_standby_durn_u8);
/**************************************************************/
/**\name	FUNCTION FOR WORK MODE   */
/**************************************************************/
/*!
 *	@brief This API is used to write
 *	 the working mode of the sensor
 *
 *
 *  @param v_work_mode_u8 : The value of work mode
 *   value      |  mode
 * -------------|-------------
 *    0         | BMP280_ULTRA_LOW_POWER_MODE
 *    1         | BMP280_LOW_POWER_MODE
 *    2         | BMP280_STANDARD_RESOLUTION_MODE
 *    3         | BMP280_HIGH_RESOLUTION_MODE
 *    4         | BMP280_ULTRA_HIGH_RESOLUTION_MODE
 *
 *  @return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
int8_t bmp280_set_work_mode(uint8_t v_work_mode_u8);
/**************************************************************/
/**\name	FUNCTION FOR FORCE MODE READING    */
/**************************************************************/
/*!
 *	@brief This API used to read both
 *	uncompensated pressure and temperature in forced mode
 *
 *
 *  @param  v_uncomp_pressure_s32: The value of uncompensated pressure.
 *  @param  v_uncomp_temperature_s32: The value of uncompensated temperature
 *
 *
 *  @return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
int8_t bmp280_get_forced_uncomp_pressure_temperature(
		int32_t *v_uncomp_pressure_s32, int32_t *v_uncomp_temperature_s32);
/**************************************************************/
/**\name	FUNCTION FOR COMMON READ AND WRITE    */
/**************************************************************/
/*!
 * @brief
 *	This API write the data to
 *	the given register
 *
 *
 *	@param v_addr_u8 -> Address of the register
 *	@param v_data_u8 -> The data from the register
 *	@param v_len_u8 -> no of bytes to read
 *
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
 */
int8_t bmp280_write_register(uint8_t v_addr_u8,
		uint8_t *v_data_u8, uint8_t v_len_u8);
/*!
 * @brief
 *	This API reads the data from
 *	the given register
 *
 *
 *	@param v_addr_u8 -> Address of the register
 *	@param v_data_u8 -> The data from the register
 *	@param v_len_u8 -> no of bytes to read
 *
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
 */
int8_t bmp280_read_register(uint8_t v_addr_u8,
		uint8_t *v_data_u8, uint8_t v_len_u8);
/**************************************************************/
/**\name	FUNCTION FOR TRUE TEMPERATURE CALCULATION   */
/**************************************************************/
#ifdef BMP280_ENABLE_FLOAT
/*!
 * @brief This API used to read
 * actual temperature from uncompensated temperature
 * @note Returns the value in Degree centigrade
 * @note Output value of "51.23" equals 51.23 DegC.
 *
 *
 *
 *  @param v_uncomp_temperature_s32 : value of uncompensated temperature
 *
 *
 *
 *  @return
 *	Actual temperature in floating point
 *
*/
double bmp280_compensate_temperature_double(int32_t v_uncomp_temperature_s32);
/**************************************************************/
/**\name	FUNCTION FOR TRUE PRESSURE CALCULATION   */
/**************************************************************/
/*!
 *	@brief Reads actual pressure from uncompensated pressure
 *	and returns pressure in Pa as double.
 *	@note Output value of "96386.2"
 *	equals 96386.2 Pa = 963.862 hPa.
 *
 *
 *
 *  @param v_uncomp_pressure_s32 : value of uncompensated pressure
 *
 *
 *
 *  @return
 *	Actual pressure in floating point
 *
*/
double bmp280_compensate_pressure_double(int32_t v_uncomp_pressure_s32);
#endif
#if defined(BMP280_ENABLE_INT64) && defined(BMP280_64BITSUPPORT_PRESENT)
/*!
 * @brief This API used to read actual pressure from uncompensated pressure
 * @note returns the value in Pa as unsigned 32 bit
 * integer in Q24.8 format (24 integer bits and
 * 8 fractional bits). Output value of "24674867"
 * represents 24674867 / 256 = 96386.2 Pa = 963.862 hPa
 *
 *
 *
 *  @param v_uncomp_pressure_s32 : value of uncompensated pressure
 *
 *
 *
 *  @return actual pressure as 64bit output
 *
*/
uint32_t bmp280_compensate_pressure_int64(int32_t v_uncomp_pressure_s32);
#endif
/**************************************************************/
/**\name	FUNCTION FOR DELAY CALCULATION DURING FORCEMODE  */
/**************************************************************/
/*!
 * @brief Computing waiting time for sensor data read
 *
 *
 *
 *
 *  @param v_delaytime_u8r: The value of delay time
 *
 *
 *  @return 0
 *
 *
 */
int8_t bmp280_compute_wait_time(uint8_t
		*v_delaytime_u8r);
#endif
