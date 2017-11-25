
#include "bmp280_registers.h"
#include "bmp280.h"

static bmp280_t *p_bmp280; /**< pointer to BMP280 */

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
int8_t bmp280_init(bmp280_t *bmp280)
{
	/* variable used to return communication result*/
	int8_t com_rslt = ERROR;
	uint8_t v_data_u8 = BMP280_INIT_VALUE;
	uint8_t v_chip_id_read_count = BMP280_CHIP_ID_READ_COUNT;

	p_bmp280 = bmp280;/* assign BMP280 ptr */

	while (v_chip_id_read_count > 0) {
		/* read chip id */
		com_rslt = p_bmp280->BMP280_BUS_READ_FUNC(p_bmp280->dev_addr,
				BMP280_CHIP_ID_REG, &v_data_u8,
				BMP280_GEN_READ_WRITE_DATA_LENGTH);
		/* Check for the correct chip id */
		if ((v_data_u8 == BMP280_CHIP_ID1)
			|| (v_data_u8 == BMP280_CHIP_ID2)
			|| (v_data_u8 == BMP280_CHIP_ID3))
			break;
		v_chip_id_read_count--;
		/* Delay added concerning the low speed of power up system to
		facilitate the proper reading of the chip ID */
		p_bmp280->delay_msec(BMP280_REGISTER_READ_DELAY);
	}

	/*assign chip ID to the global structure*/
	p_bmp280->chip_id = v_data_u8;
	/*com_rslt status of chip ID read*/
	com_rslt = (v_chip_id_read_count == BMP280_INIT_VALUE) ?
			BMP280_CHIP_ID_READ_FAIL : BMP280_CHIP_ID_READ_SUCCESS;

	if (com_rslt == BMP280_CHIP_ID_READ_SUCCESS) {
		/* readout bmp280 calibration parameter structure */
		com_rslt += bmp280_get_calib_param();
	}
	return com_rslt;
}
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
		int32_t *v_uncomp_temperature_s32)
{
	/* variable used to return communication result*/
	int8_t com_rslt = ERROR;
	/* Array holding the MSB and LSb value
	 a_data_u8r[0] - Temperature MSB
	 a_data_u8r[1] - Temperature LSB
	 a_data_u8r[2] - Temperature LSB
	 */
	uint8_t a_data_u8r[BMP280_TEMPERATURE_DATA_SIZE] = {BMP280_INIT_VALUE,
			BMP280_INIT_VALUE, BMP280_INIT_VALUE};
	/* check the p_bmp280 structure pointer as NULL*/
	if (p_bmp280 == BMP280_NULL) {
		com_rslt = E_BMP280_NULL_PTR;
	} else {
		/* read temperature data */
		com_rslt = p_bmp280->BMP280_BUS_READ_FUNC(p_bmp280->dev_addr,
				BMP280_TEMPERATURE_MSB_REG, a_data_u8r,
				BMP280_TEMPERATURE_DATA_LENGTH);
		*v_uncomp_temperature_s32 = (int32_t)((((uint32_t)(
				a_data_u8r[BMP280_TEMPERATURE_MSB_DATA]))
				<< BMP280_SHIFT_BIT_POSITION_BY_12_BITS)
				| (((uint32_t)(
				a_data_u8r[BMP280_TEMPERATURE_LSB_DATA]))
				<< BMP280_SHIFT_BIT_POSITION_BY_04_BITS)
				| ((uint32_t)a_data_u8r[BMP280_TEMPERATURE_XLSB_DATA]
				>> BMP280_SHIFT_BIT_POSITION_BY_04_BITS));
	}
	return com_rslt;
}
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
int32_t bmp280_compensate_temperature_int32(int32_t v_uncomp_temperature_s32)
{
	int32_t v_x1_u32r = BMP280_INIT_VALUE;
	int32_t v_x2_u32r = BMP280_INIT_VALUE;
	int32_t temperature = BMP280_INIT_VALUE;
	/* calculate true temperature*/
	/*calculate x1*/
	v_x1_u32r = ((((v_uncomp_temperature_s32
			>> BMP280_SHIFT_BIT_POSITION_BY_03_BITS)
			- ((int32_t)p_bmp280->calib_param.dig_T1
			<< BMP280_SHIFT_BIT_POSITION_BY_01_BIT)))
			* ((int32_t)p_bmp280->calib_param.dig_T2))
			>> BMP280_SHIFT_BIT_POSITION_BY_11_BITS;
	/*calculate x2*/
	v_x2_u32r = (((((v_uncomp_temperature_s32
			>> BMP280_SHIFT_BIT_POSITION_BY_04_BITS)
			- ((int32_t)p_bmp280->calib_param.dig_T1))
			* ((v_uncomp_temperature_s32
			>> BMP280_SHIFT_BIT_POSITION_BY_04_BITS)
			- ((int32_t)p_bmp280->calib_param.dig_T1)))
			>> BMP280_SHIFT_BIT_POSITION_BY_12_BITS)
			* ((int32_t)p_bmp280->calib_param.dig_T3))
			>> BMP280_SHIFT_BIT_POSITION_BY_14_BITS;
	/*calculate t_fine*/
	p_bmp280->calib_param.t_fine = v_x1_u32r + v_x2_u32r;
	/*calculate temperature*/
	temperature = (p_bmp280->calib_param.t_fine * 5 + 128)
			>> BMP280_SHIFT_BIT_POSITION_BY_08_BITS;

	return temperature;
}
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
		int32_t *v_uncomp_pressure_s32)
{
	/* variable used to return communication result*/
	int8_t com_rslt = ERROR;
	/* Array holding the MSB and LSb value
	 a_data_u8[0] - Pressure MSB
	 a_data_u8[1] - Pressure LSB
	 a_data_u8[2] - Pressure LSB
	 */
	uint8_t a_data_u8[BMP280_PRESSURE_DATA_SIZE] = {BMP280_INIT_VALUE,
			BMP280_INIT_VALUE, BMP280_INIT_VALUE};
	/* check the p_bmp280 structure pointer as NULL*/
	if (p_bmp280 == BMP280_NULL) {
		com_rslt = E_BMP280_NULL_PTR;
	} else {
		com_rslt = p_bmp280->BMP280_BUS_READ_FUNC(p_bmp280->dev_addr,
				BMP280_PRESSURE_MSB_REG, a_data_u8,
				BMP280_PRESSURE_DATA_LENGTH);
		*v_uncomp_pressure_s32 = (int32_t)((((uint32_t)(
				a_data_u8[BMP280_PRESSURE_MSB_DATA]))
				<< BMP280_SHIFT_BIT_POSITION_BY_12_BITS)
				| (((uint32_t)(a_data_u8[BMP280_PRESSURE_LSB_DATA]))
				<< BMP280_SHIFT_BIT_POSITION_BY_04_BITS)
				| ((uint32_t)a_data_u8[BMP280_PRESSURE_XLSB_DATA]
				>> BMP280_SHIFT_BIT_POSITION_BY_04_BITS));
	}
	return com_rslt;
}
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
uint32_t bmp280_compensate_pressure_int32(int32_t v_uncomp_pressure_s32)
{
	int32_t v_x1_u32r = BMP280_INIT_VALUE;
	int32_t v_x2_u32r = BMP280_INIT_VALUE;
	uint32_t v_pressure_u32 = BMP280_INIT_VALUE;
	/* calculate x1*/
	v_x1_u32r = (((int32_t)p_bmp280->calib_param.t_fine)
			>> BMP280_SHIFT_BIT_POSITION_BY_01_BIT) - (int32_t)64000;
	/* calculate x2*/
	v_x2_u32r = (((v_x1_u32r >> BMP280_SHIFT_BIT_POSITION_BY_02_BITS)
			* (v_x1_u32r >> BMP280_SHIFT_BIT_POSITION_BY_02_BITS))
			>> BMP280_SHIFT_BIT_POSITION_BY_11_BITS)
			* ((int32_t)p_bmp280->calib_param.dig_P6);
	v_x2_u32r = v_x2_u32r + ((v_x1_u32r *
			((int32_t)p_bmp280->calib_param.dig_P5))
			<< BMP280_SHIFT_BIT_POSITION_BY_01_BIT);
	v_x2_u32r = (v_x2_u32r >> BMP280_SHIFT_BIT_POSITION_BY_02_BITS)
			+ (((int32_t)p_bmp280->calib_param.dig_P4)
			<< BMP280_SHIFT_BIT_POSITION_BY_16_BITS);
	/* calculate x1*/
	v_x1_u32r = (((p_bmp280->calib_param.dig_P3
			* (((v_x1_u32r
			>> BMP280_SHIFT_BIT_POSITION_BY_02_BITS) * (v_x1_u32r
			>> BMP280_SHIFT_BIT_POSITION_BY_02_BITS))
			>> BMP280_SHIFT_BIT_POSITION_BY_13_BITS))
			>> BMP280_SHIFT_BIT_POSITION_BY_03_BITS)
			+ ((((int32_t)p_bmp280->calib_param.dig_P2)
			* v_x1_u32r)
			>> BMP280_SHIFT_BIT_POSITION_BY_01_BIT))
			>> BMP280_SHIFT_BIT_POSITION_BY_18_BITS;
	v_x1_u32r = ((((32768 + v_x1_u32r))
			* ((int32_t)p_bmp280->calib_param.dig_P1))
			>> BMP280_SHIFT_BIT_POSITION_BY_15_BITS);
	/* calculate pressure*/
	v_pressure_u32 = (((uint32_t)(((int32_t)1048576) - v_uncomp_pressure_s32)
			- (v_x2_u32r >> BMP280_SHIFT_BIT_POSITION_BY_12_BITS)))
			* 3125;
	/* check overflow*/
	if (v_pressure_u32 < 0x80000000)
		/* Avoid exception caused by division by zero */
		if (v_x1_u32r != BMP280_INIT_VALUE)
			v_pressure_u32 = (v_pressure_u32
					<< BMP280_SHIFT_BIT_POSITION_BY_01_BIT)
					/ ((uint32_t)v_x1_u32r);
		else
			return BMP280_INVALID_DATA;
	else
	/* Avoid exception caused by division by zero */
	if (v_x1_u32r != BMP280_INIT_VALUE)
		v_pressure_u32 = (v_pressure_u32 / (uint32_t)v_x1_u32r) * 2;
	else
		return BMP280_INVALID_DATA;
	/* calculate x1*/
	v_x1_u32r = (((int32_t)p_bmp280->calib_param.dig_P9) * ((int32_t)(
			((v_pressure_u32
			>> BMP280_SHIFT_BIT_POSITION_BY_03_BITS)
			* (v_pressure_u32
			>> BMP280_SHIFT_BIT_POSITION_BY_03_BITS))
			>> BMP280_SHIFT_BIT_POSITION_BY_13_BITS)))
			>> BMP280_SHIFT_BIT_POSITION_BY_12_BITS;
	/* calculate x2*/
	v_x2_u32r = (((int32_t)(v_pressure_u32 >>
			BMP280_SHIFT_BIT_POSITION_BY_02_BITS))
			* ((int32_t)p_bmp280->calib_param.dig_P8))
			>> BMP280_SHIFT_BIT_POSITION_BY_13_BITS;
	/* calculate true pressure*/
	v_pressure_u32 = (uint32_t)((int32_t)v_pressure_u32 + ((v_x1_u32r + v_x2_u32r
			+ p_bmp280->calib_param.dig_P7)
			>> BMP280_SHIFT_BIT_POSITION_BY_04_BITS));

	return v_pressure_u32;
}
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
		int32_t *v_uncomp_pressure_s32, int32_t *v_uncomp_temperature_s32)
{
	/* variable used to return communication result*/
	int8_t com_rslt = ERROR;
	/* Array holding the temperature and pressure data
	 a_data_u8[0] - Pressure MSB
	 a_data_u8[1] - Pressure LSB
	 a_data_u8[2] - Pressure LSB
	 a_data_u8[3] - Temperature MSB
	 a_data_u8[4] - Temperature LSB
	 a_data_u8[5] - Temperature LSB
	 */
	uint8_t a_data_u8[BMP280_ALL_DATA_FRAME_LENGTH] = {BMP280_INIT_VALUE,
			BMP280_INIT_VALUE, BMP280_INIT_VALUE, BMP280_INIT_VALUE,
			BMP280_INIT_VALUE, BMP280_INIT_VALUE};
	/* check the p_bmp280 structure pointer as NULL*/
	if (p_bmp280 == BMP280_NULL) {
		com_rslt = E_BMP280_NULL_PTR;
	} else {
		com_rslt = p_bmp280->BMP280_BUS_READ_FUNC(p_bmp280->dev_addr,
				BMP280_PRESSURE_MSB_REG, a_data_u8,
				BMP280_DATA_FRAME_SIZE);
		/*Pressure*/
		*v_uncomp_pressure_s32 = (int32_t)((((uint32_t)(
				a_data_u8[BMP280_DATA_FRAME_PRESSURE_MSB_BYTE]))
				<< BMP280_SHIFT_BIT_POSITION_BY_12_BITS)
				| (((uint32_t)(
				a_data_u8[BMP280_DATA_FRAME_PRESSURE_LSB_BYTE]))
				<< BMP280_SHIFT_BIT_POSITION_BY_04_BITS)
				| ((uint32_t)a_data_u8[
				BMP280_DATA_FRAME_PRESSURE_XLSB_BYTE]
				>> BMP280_SHIFT_BIT_POSITION_BY_04_BITS));

		/* Temperature */
		*v_uncomp_temperature_s32 = (int32_t)((((uint32_t)(a_data_u8[
				BMP280_DATA_FRAME_TEMPERATURE_MSB_BYTE]))
				<< BMP280_SHIFT_BIT_POSITION_BY_12_BITS)
				| (((uint32_t)(a_data_u8[
				BMP280_DATA_FRAME_TEMPERATURE_LSB_BYTE]))
				<< BMP280_SHIFT_BIT_POSITION_BY_04_BITS)
				| ((uint32_t)a_data_u8[
				BMP280_DATA_FRAME_TEMPERATURE_XLSB_BYTE]
				>> BMP280_SHIFT_BIT_POSITION_BY_04_BITS));
	}
	return com_rslt;
}
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
		uint32_t *v_pressure_u32, int32_t *v_temperature_s32)
{
	/* variable used to return communication result*/
	int8_t com_rslt = ERROR;
	int32_t v_uncomp_pressure_s32 = BMP280_INIT_VALUE;
	int32_t v_uncomp_temperature_s32 = BMP280_INIT_VALUE;
	/* check the p_bmp280 structure pointer as NULL*/
	if (p_bmp280 == BMP280_NULL) {
		com_rslt = E_BMP280_NULL_PTR;
	} else {
		/* read uncompensated pressure and temperature*/
		com_rslt = bmp280_read_uncomp_pressure_temperature(
				&v_uncomp_pressure_s32,
				&v_uncomp_temperature_s32);
		/* read true pressure and temperature*/
		*v_temperature_s32 = bmp280_compensate_temperature_int32(
				v_uncomp_temperature_s32);
		*v_pressure_u32 = bmp280_compensate_pressure_int32(
				v_uncomp_pressure_s32);
	}
	return com_rslt;
}
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
int8_t bmp280_get_calib_param(void)
{
	/* variable used to return communication result*/
	int8_t com_rslt = ERROR;
	uint8_t a_data_u8[BMP280_CALIB_DATA_SIZE] = {BMP280_INIT_VALUE,
			BMP280_INIT_VALUE, BMP280_INIT_VALUE, BMP280_INIT_VALUE,
			BMP280_INIT_VALUE, BMP280_INIT_VALUE, BMP280_INIT_VALUE,
			BMP280_INIT_VALUE, BMP280_INIT_VALUE, BMP280_INIT_VALUE,
			BMP280_INIT_VALUE, BMP280_INIT_VALUE, BMP280_INIT_VALUE,
			BMP280_INIT_VALUE, BMP280_INIT_VALUE, BMP280_INIT_VALUE,
			BMP280_INIT_VALUE, BMP280_INIT_VALUE, BMP280_INIT_VALUE,
			BMP280_INIT_VALUE, BMP280_INIT_VALUE, BMP280_INIT_VALUE,
			BMP280_INIT_VALUE, BMP280_INIT_VALUE};
	/* check the p_bmp280 structure pointer as NULL*/
	if (p_bmp280 == BMP280_NULL) {
		com_rslt = E_BMP280_NULL_PTR;
	} else {
		com_rslt = p_bmp280->BMP280_BUS_READ_FUNC(p_bmp280->dev_addr,
				BMP280_TEMPERATURE_CALIB_DIG_T1_LSB_REG,
				a_data_u8,
				BMP280_PRESSURE_TEMPERATURE_CALIB_DATA_LENGTH);
		/* read calibration values*/
		p_bmp280->calib_param.dig_T1 = (uint16_t)((((uint16_t)((uint8_t)a_data_u8[
					BMP280_TEMPERATURE_CALIB_DIG_T1_MSB]))
					<< BMP280_SHIFT_BIT_POSITION_BY_08_BITS)
					| a_data_u8[
					BMP280_TEMPERATURE_CALIB_DIG_T1_LSB]);
		p_bmp280->calib_param.dig_T2 = (int16_t)((((int16_t)((int8_t)a_data_u8[
					BMP280_TEMPERATURE_CALIB_DIG_T2_MSB]))
					<< BMP280_SHIFT_BIT_POSITION_BY_08_BITS)
					| a_data_u8[
					BMP280_TEMPERATURE_CALIB_DIG_T2_LSB]);
		p_bmp280->calib_param.dig_T3 = (int16_t)((((int16_t)((int8_t)a_data_u8[
					BMP280_TEMPERATURE_CALIB_DIG_T3_MSB]))
					<< BMP280_SHIFT_BIT_POSITION_BY_08_BITS)
					| a_data_u8[
					BMP280_TEMPERATURE_CALIB_DIG_T3_LSB]);
		p_bmp280->calib_param.dig_P1 = (uint16_t)((((uint16_t)((uint8_t)a_data_u8[
					BMP280_PRESSURE_CALIB_DIG_P1_MSB]))
					<< BMP280_SHIFT_BIT_POSITION_BY_08_BITS)
					| a_data_u8[
					BMP280_PRESSURE_CALIB_DIG_P1_LSB]);
		p_bmp280->calib_param.dig_P2 = (int16_t)((((int16_t)((int8_t)a_data_u8[
					BMP280_PRESSURE_CALIB_DIG_P2_MSB]))
					<< BMP280_SHIFT_BIT_POSITION_BY_08_BITS)
					| a_data_u8[
					BMP280_PRESSURE_CALIB_DIG_P2_LSB]);
		p_bmp280->calib_param.dig_P3 = (int16_t)((((int16_t)((int8_t)a_data_u8[
					BMP280_PRESSURE_CALIB_DIG_P3_MSB]))
					<< BMP280_SHIFT_BIT_POSITION_BY_08_BITS)
					| a_data_u8[
					BMP280_PRESSURE_CALIB_DIG_P3_LSB]);
		p_bmp280->calib_param.dig_P4 = (int16_t)((((int16_t)((int8_t)a_data_u8[
					BMP280_PRESSURE_CALIB_DIG_P4_MSB]))
					<< BMP280_SHIFT_BIT_POSITION_BY_08_BITS)
					| a_data_u8[
					BMP280_PRESSURE_CALIB_DIG_P4_LSB]);
		p_bmp280->calib_param.dig_P5 = (int16_t)((((int16_t)((int8_t)a_data_u8[
					BMP280_PRESSURE_CALIB_DIG_P5_MSB]))
					<< BMP280_SHIFT_BIT_POSITION_BY_08_BITS)
					| a_data_u8[
					BMP280_PRESSURE_CALIB_DIG_P5_LSB]);
		p_bmp280->calib_param.dig_P6 = (int16_t)((((int16_t)((int8_t)a_data_u8[
					BMP280_PRESSURE_CALIB_DIG_P6_MSB]))
					<< BMP280_SHIFT_BIT_POSITION_BY_08_BITS)
					| a_data_u8[
					BMP280_PRESSURE_CALIB_DIG_P6_LSB]);
		p_bmp280->calib_param.dig_P7 = (int16_t)((((int16_t)((int8_t)a_data_u8[
					BMP280_PRESSURE_CALIB_DIG_P7_MSB]))
					<< BMP280_SHIFT_BIT_POSITION_BY_08_BITS)
					| a_data_u8[
					BMP280_PRESSURE_CALIB_DIG_P7_LSB]);
		p_bmp280->calib_param.dig_P8 = (int16_t)((((int16_t)((int8_t)a_data_u8[
					BMP280_PRESSURE_CALIB_DIG_P8_MSB]))
					<< BMP280_SHIFT_BIT_POSITION_BY_08_BITS)
					| a_data_u8[
					BMP280_PRESSURE_CALIB_DIG_P8_LSB]);
		p_bmp280->calib_param.dig_P9 = (int16_t)((((int16_t)((int8_t)a_data_u8[
					BMP280_PRESSURE_CALIB_DIG_P9_MSB]))
					<< BMP280_SHIFT_BIT_POSITION_BY_08_BITS)
					| a_data_u8[
					BMP280_PRESSURE_CALIB_DIG_P9_LSB]);
	}
	return com_rslt;
}
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
int8_t bmp280_get_oversamp_temperature(uint8_t *v_value_u8)
{
	/* variable used to return communication result*/
	int8_t com_rslt = ERROR;
	uint8_t v_data_u8 = BMP280_INIT_VALUE;
	/* check the p_bmp280 structure pointer as NULL*/
	if (p_bmp280 == BMP280_NULL) {
		com_rslt = E_BMP280_NULL_PTR;
	} else {
		/* read temperature over sampling*/
		com_rslt = p_bmp280->BMP280_BUS_READ_FUNC(p_bmp280->dev_addr,
				BMP280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE__REG,
				&v_data_u8, BMP280_GEN_READ_WRITE_DATA_LENGTH);
		*v_value_u8 = BMP280_GET_BITSLICE(v_data_u8,
				BMP280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE);
		/* assign temperature oversampling*/
		p_bmp280->oversamp_temperature = *v_value_u8;
	}
	return com_rslt;
}
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
int8_t bmp280_set_oversamp_temperature(uint8_t v_value_u8)
{
	/* variable used to return communication result*/
	int8_t com_rslt = ERROR;
	uint8_t v_data_u8 = BMP280_INIT_VALUE;
	/* check the p_bmp280 structure pointer as NULL*/
	if (p_bmp280 == BMP280_NULL) {
		com_rslt = E_BMP280_NULL_PTR;
	} else {
		com_rslt = p_bmp280->BMP280_BUS_READ_FUNC(p_bmp280->dev_addr,
				BMP280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE__REG,
				&v_data_u8, BMP280_GEN_READ_WRITE_DATA_LENGTH);
		if (com_rslt == SUCCESS) {
			/* write over sampling*/
			v_data_u8 = BMP280_SET_BITSLICE(v_data_u8,
				BMP280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE,
				v_value_u8);
			com_rslt += p_bmp280->BMP280_BUS_WRITE_FUNC(
				p_bmp280->dev_addr,
				BMP280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE__REG,
				&v_data_u8,
				BMP280_GEN_READ_WRITE_DATA_LENGTH);
			p_bmp280->oversamp_temperature = v_value_u8;
		}
	}
	return com_rslt;
}
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
int8_t bmp280_get_oversamp_pressure(uint8_t *v_value_u8)
{
	/* variable used to return communication result*/
	int8_t com_rslt = ERROR;
	uint8_t v_data_u8 = BMP280_INIT_VALUE;
	/* check the p_bmp280 structure pointer as NULL*/
	if (p_bmp280 == BMP280_NULL) {
		com_rslt = E_BMP280_NULL_PTR;
	} else {
		/* read pressure over sampling */
		com_rslt = p_bmp280->BMP280_BUS_READ_FUNC(p_bmp280->dev_addr,
				BMP280_CTRL_MEAS_REG_OVERSAMP_PRESSURE__REG,
				&v_data_u8, BMP280_GEN_READ_WRITE_DATA_LENGTH);
		*v_value_u8 = BMP280_GET_BITSLICE(v_data_u8,
				BMP280_CTRL_MEAS_REG_OVERSAMP_PRESSURE);

		p_bmp280->oversamp_pressure = *v_value_u8;
	}
	return com_rslt;
}
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
int8_t bmp280_set_oversamp_pressure(uint8_t v_value_u8)
{
	/* variable used to return communication result*/
	int8_t com_rslt = ERROR;
	uint8_t v_data_u8 = BMP280_INIT_VALUE;
	/* check the p_bmp280 structure pointer as NULL*/
	if (p_bmp280 == BMP280_NULL) {
		com_rslt = E_BMP280_NULL_PTR;
	} else {
		com_rslt = p_bmp280->BMP280_BUS_READ_FUNC(p_bmp280->dev_addr,
				BMP280_CTRL_MEAS_REG_OVERSAMP_PRESSURE__REG,
				&v_data_u8, BMP280_GEN_READ_WRITE_DATA_LENGTH);
		if (com_rslt == SUCCESS) {
			/* write pressure over sampling */
			v_data_u8 = BMP280_SET_BITSLICE(v_data_u8,
					BMP280_CTRL_MEAS_REG_OVERSAMP_PRESSURE,
					v_value_u8);
			com_rslt += p_bmp280->BMP280_BUS_WRITE_FUNC(
				p_bmp280->dev_addr,
				BMP280_CTRL_MEAS_REG_OVERSAMP_PRESSURE__REG,
				&v_data_u8,
				BMP280_GEN_READ_WRITE_DATA_LENGTH);

			p_bmp280->oversamp_pressure = v_value_u8;
		}
	}
	return com_rslt;
}
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
int8_t bmp280_get_power_mode(uint8_t *v_power_mode_u8)
{
	/* variable used to return communication result*/
	int8_t com_rslt = ERROR;
	uint8_t v_mode_u8 = BMP280_INIT_VALUE;
	/* check the p_bmp280 structure pointer as NULL*/
	if (p_bmp280 == BMP280_NULL) {
		com_rslt = E_BMP280_NULL_PTR;
	} else {
		/* read the power mode*/
		com_rslt = p_bmp280->BMP280_BUS_READ_FUNC(p_bmp280->dev_addr,
				BMP280_CTRL_MEAS_REG_POWER_MODE__REG,
				&v_mode_u8, BMP280_GEN_READ_WRITE_DATA_LENGTH);
		*v_power_mode_u8 = BMP280_GET_BITSLICE(v_mode_u8,
				BMP280_CTRL_MEAS_REG_POWER_MODE);
	}
	return com_rslt;
}
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
int8_t bmp280_set_power_mode(uint8_t v_power_mode_u8)
{
	/* variable used to return communication result*/
	int8_t com_rslt = ERROR;
	uint8_t v_mode_u8 = BMP280_INIT_VALUE;
	/* check the p_bmp280 structure pointer as NULL*/
	if (p_bmp280 == BMP280_NULL) {
		com_rslt = E_BMP280_NULL_PTR;
	} else {
		if (v_power_mode_u8 <= BMP280_NORMAL_MODE) {
			/* write the power mode*/
			v_mode_u8 = (p_bmp280->oversamp_temperature
					<< BMP280_SHIFT_BIT_POSITION_BY_05_BITS)
					+ (p_bmp280->oversamp_pressure
					<< BMP280_SHIFT_BIT_POSITION_BY_02_BITS)
					+ v_power_mode_u8;
			com_rslt = p_bmp280->BMP280_BUS_WRITE_FUNC(
					p_bmp280->dev_addr,
					BMP280_CTRL_MEAS_REG_POWER_MODE__REG,
					&v_mode_u8,
					BMP280_GEN_READ_WRITE_DATA_LENGTH);
		} else {
			com_rslt = E_BMP280_OUT_OF_RANGE;
		}
	}
	return com_rslt;
}
/*!
 * @brief Used to reset the sensor
 * The value 0xB6 is written to the 0xE0 register
 * the device is reset using the
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
int8_t bmp280_set_soft_rst(void)
{
	/* variable used to return communication result*/
	int8_t com_rslt = ERROR;
	uint8_t v_data_u8 = BMP280_SOFT_RESET_CODE;
	/* check the p_bmp280 structure pointer as NULL*/
	if (p_bmp280 == BMP280_NULL) {
		com_rslt = E_BMP280_NULL_PTR;
	} else {
		/* write soft reset */
		com_rslt = p_bmp280->BMP280_BUS_WRITE_FUNC(p_bmp280->dev_addr,
				BMP280_RST_REG, &v_data_u8,
				BMP280_GEN_READ_WRITE_DATA_LENGTH);
	}
	return com_rslt;
}
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
int8_t bmp280_get_spi3(uint8_t *v_enable_disable_u8)
{
	/* variable used to return communication result*/
	int8_t com_rslt = ERROR;
	uint8_t v_data_u8 = BMP280_INIT_VALUE;
	/* check the p_bmp280 structure pointer as NULL*/
	if (p_bmp280 == BMP280_NULL) {
		com_rslt = E_BMP280_NULL_PTR;
	} else {
		com_rslt = p_bmp280->BMP280_BUS_READ_FUNC(p_bmp280->dev_addr,
				BMP280_CONFIG_REG_SPI3_ENABLE__REG, &v_data_u8,
				BMP280_GEN_READ_WRITE_DATA_LENGTH);
		*v_enable_disable_u8 = BMP280_GET_BITSLICE(v_data_u8,
				BMP280_CONFIG_REG_SPI3_ENABLE);
	}
	return com_rslt;
}
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
int8_t bmp280_set_spi3(uint8_t v_enable_disable_u8)
{
	/* variable used to return communication result*/
	int8_t com_rslt = ERROR;
	uint8_t v_data_u8 = BMP280_INIT_VALUE;
	/* check the p_bmp280 structure pointer as NULL*/
	if (p_bmp280 == BMP280_NULL) {
		com_rslt = E_BMP280_NULL_PTR;
	} else {
		com_rslt = p_bmp280->BMP280_BUS_READ_FUNC(p_bmp280->dev_addr,
				BMP280_CONFIG_REG_SPI3_ENABLE__REG, &v_data_u8,
				BMP280_GEN_READ_WRITE_DATA_LENGTH);
		if (com_rslt == SUCCESS) {
			v_data_u8 = BMP280_SET_BITSLICE(v_data_u8,
					BMP280_CONFIG_REG_SPI3_ENABLE,
					v_enable_disable_u8);
			com_rslt += p_bmp280->BMP280_BUS_WRITE_FUNC(
					p_bmp280->dev_addr,
					BMP280_CONFIG_REG_SPI3_ENABLE__REG,
					&v_data_u8,
					BMP280_GEN_READ_WRITE_DATA_LENGTH);
		}
	}
	return com_rslt;
}
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
int8_t bmp280_get_filter(uint8_t *v_value_u8)
{
	/* variable used to return communication result*/
	int8_t com_rslt = ERROR;
	uint8_t v_data_u8 = BMP280_INIT_VALUE;
	/* check the p_bmp280 structure pointer as NULL*/
	if (p_bmp280 == BMP280_NULL) {
		com_rslt = E_BMP280_NULL_PTR;
	} else {
		/* read filter*/
		com_rslt = p_bmp280->BMP280_BUS_READ_FUNC(p_bmp280->dev_addr,
				BMP280_CONFIG_REG_FILTER__REG, &v_data_u8,
				BMP280_GEN_READ_WRITE_DATA_LENGTH);
		*v_value_u8 = BMP280_GET_BITSLICE(v_data_u8,
				BMP280_CONFIG_REG_FILTER);
	}
	return com_rslt;
}
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
int8_t bmp280_set_filter(uint8_t v_value_u8)
{
	int8_t com_rslt = SUCCESS;
	uint8_t v_data_u8 = BMP280_INIT_VALUE;
	/* check the p_bmp280 structure pointer as NULL*/
	if (p_bmp280 == BMP280_NULL) {
		com_rslt = E_BMP280_NULL_PTR;
	} else {
		/* write filter*/
		com_rslt = p_bmp280->BMP280_BUS_READ_FUNC(p_bmp280->dev_addr,
				BMP280_CONFIG_REG_FILTER__REG, &v_data_u8,
				BMP280_GEN_READ_WRITE_DATA_LENGTH);
		if (com_rslt == SUCCESS) {
			v_data_u8 = BMP280_SET_BITSLICE(v_data_u8,
					BMP280_CONFIG_REG_FILTER, v_value_u8);
			com_rslt += p_bmp280->BMP280_BUS_WRITE_FUNC(
					p_bmp280->dev_addr,
					BMP280_CONFIG_REG_FILTER__REG,
					&v_data_u8,
					BMP280_GEN_READ_WRITE_DATA_LENGTH);
		}
	}
	return com_rslt;
}
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
int8_t bmp280_get_standby_durn(uint8_t *v_standby_durn_u8)
{
	/* variable used to return communication result*/
	int8_t com_rslt = ERROR;
	uint8_t v_data_u8 = BMP280_INIT_VALUE;
	/* check the p_bmp280 structure pointer as NULL*/
	if (p_bmp280 == BMP280_NULL) {
		com_rslt = E_BMP280_NULL_PTR;
	} else {
		/* read the standby duration*/
		com_rslt = p_bmp280->BMP280_BUS_READ_FUNC(p_bmp280->dev_addr,
				BMP280_CONFIG_REG_STANDBY_DURN__REG, &v_data_u8,
				BMP280_GEN_READ_WRITE_DATA_LENGTH);
		*v_standby_durn_u8 = BMP280_GET_BITSLICE(v_data_u8,
				BMP280_CONFIG_REG_STANDBY_DURN);
	}
	return com_rslt;
}
/*!
 *	@brief This API used to Read the
 *	standby duration time from the sensor
 *	in the register 0xF5 bit 5 to 7
 *	@note Normal mode comprises an
 *	automated perpetual cycling between an (active)
 *	Measurement period and an (inactive) standby period.
 *	@note The standby time is determined
 *	by the contents of the register t_sb.
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
int8_t bmp280_set_standby_durn(uint8_t v_standby_durn_u8)
{
	/* variable used to return communication result*/
	int8_t com_rslt = ERROR;
	uint8_t v_data_u8 = BMP280_INIT_VALUE;
	/* check the p_bmp280 structure pointer as NULL*/
	if (p_bmp280 == BMP280_NULL) {
		com_rslt = E_BMP280_NULL_PTR;
	} else {
		/* write the standby duration*/
		com_rslt = p_bmp280->BMP280_BUS_READ_FUNC(p_bmp280->dev_addr,
				BMP280_CONFIG_REG_STANDBY_DURN__REG, &v_data_u8,
				BMP280_GEN_READ_WRITE_DATA_LENGTH);
		if (com_rslt == SUCCESS) {
			v_data_u8 = BMP280_SET_BITSLICE(v_data_u8,
					BMP280_CONFIG_REG_STANDBY_DURN,
					v_standby_durn_u8);
			com_rslt += p_bmp280->BMP280_BUS_WRITE_FUNC(
					p_bmp280->dev_addr,
					BMP280_CONFIG_REG_STANDBY_DURN__REG,
					&v_data_u8,
					BMP280_GEN_READ_WRITE_DATA_LENGTH);
		}
	}
	return com_rslt;
}
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
int8_t bmp280_set_work_mode(uint8_t v_work_mode_u8)
{
	/* variable used to return communication result*/
	int8_t com_rslt = ERROR;
	uint8_t v_data_u8 = BMP280_INIT_VALUE;
	/* check the p_bmp280 structure pointer as NULL*/
	if (p_bmp280 == BMP280_NULL) {
		com_rslt = E_BMP280_NULL_PTR;
	} else {
	if (v_work_mode_u8 <= BMP280_ULTRA_HIGH_RESOLUTION_MODE) {
		com_rslt = p_bmp280->BMP280_BUS_READ_FUNC(
				p_bmp280->dev_addr,
				BMP280_CTRL_MEAS_REG, &v_data_u8,
				BMP280_GEN_READ_WRITE_DATA_LENGTH);
		if (com_rslt == SUCCESS) {
			switch (v_work_mode_u8) {
			/* write work mode*/
			case BMP280_ULTRA_LOW_POWER_MODE:
				p_bmp280->oversamp_temperature =
				BMP280_ULTRALOWPOWER_OVERSAMP_TEMPERATURE;
				p_bmp280->oversamp_pressure =
					BMP280_ULTRALOWPOWER_OVERSAMP_PRESSURE;
				break;
			case BMP280_LOW_POWER_MODE:
				p_bmp280->oversamp_temperature =
					BMP280_LOWPOWER_OVERSAMP_TEMPERATURE;
				p_bmp280->oversamp_pressure =
					BMP280_LOWPOWER_OVERSAMP_PRESSURE;
				break;
			case BMP280_STANDARD_RESOLUTION_MODE:
				p_bmp280->oversamp_temperature =
				BMP280_STANDARDRESOLUTION_OVERSAMP_TEMPERATURE;
				p_bmp280->oversamp_pressure =
				BMP280_STANDARDRESOLUTION_OVERSAMP_PRESSURE;
				break;
			case BMP280_HIGH_RESOLUTION_MODE:
				p_bmp280->oversamp_temperature =
				BMP280_HIGHRESOLUTION_OVERSAMP_TEMPERATURE;
				p_bmp280->oversamp_pressure =
				BMP280_HIGHRESOLUTION_OVERSAMP_PRESSURE;
				break;
			case BMP280_ULTRA_HIGH_RESOLUTION_MODE:
				p_bmp280->oversamp_temperature =
				BMP280_ULTRAHIGHRESOLUTION_OVERSAMP_TEMPERATURE;
				p_bmp280->oversamp_pressure =
				BMP280_ULTRAHIGHRESOLUTION_OVERSAMP_PRESSURE;
				break;
			}
			v_data_u8 = BMP280_SET_BITSLICE(v_data_u8,
				BMP280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE,
				p_bmp280->oversamp_temperature);
			v_data_u8 = BMP280_SET_BITSLICE(v_data_u8,
				BMP280_CTRL_MEAS_REG_OVERSAMP_PRESSURE,
				p_bmp280->oversamp_pressure);
			com_rslt += p_bmp280->BMP280_BUS_WRITE_FUNC(
				p_bmp280->dev_addr, BMP280_CTRL_MEAS_REG,
				&v_data_u8, BMP280_GEN_READ_WRITE_DATA_LENGTH);
		}
	} else {
		com_rslt = E_BMP280_OUT_OF_RANGE;
	}
	}
	return com_rslt;
}
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
		int32_t *v_uncomp_pressure_s32, int32_t *v_uncomp_temperature_s32)
{
	/* variable used to return communication result*/
	int8_t com_rslt = ERROR;
	uint8_t v_data_u8 = BMP280_INIT_VALUE;
	uint8_t v_waittime_u8 = BMP280_INIT_VALUE;
	/* check the p_bmp280 structure pointer as NULL*/
	if (p_bmp280 == BMP280_NULL) {
		com_rslt = E_BMP280_NULL_PTR;
	} else {
		/* read pressure and temperature*/
		v_data_u8 = (p_bmp280->oversamp_temperature
				<< BMP280_SHIFT_BIT_POSITION_BY_05_BITS)
				+ (p_bmp280->oversamp_pressure
				<< BMP280_SHIFT_BIT_POSITION_BY_02_BITS)
				+ BMP280_FORCED_MODE;
		com_rslt = p_bmp280->BMP280_BUS_WRITE_FUNC(p_bmp280->dev_addr,
				BMP280_CTRL_MEAS_REG, &v_data_u8,
				BMP280_GEN_READ_WRITE_DATA_LENGTH);
		bmp280_compute_wait_time(&v_waittime_u8);
		p_bmp280->delay_msec(v_waittime_u8);
		com_rslt += bmp280_read_uncomp_pressure_temperature(
				v_uncomp_pressure_s32,
				v_uncomp_temperature_s32);
	}
	return com_rslt;
}
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
int8_t bmp280_write_register(uint8_t v_addr_u8, uint8_t *v_data_u8,
		uint8_t v_len_u8)
{
	/* variable used to return communication result*/
	int8_t com_rslt = ERROR;
	/* check the p_bmp280 structure pointer as NULL*/
	if (p_bmp280 == BMP280_NULL) {
		com_rslt = E_BMP280_NULL_PTR;
	} else {
		com_rslt = p_bmp280->BMP280_BUS_WRITE_FUNC(p_bmp280->dev_addr,
				v_addr_u8, v_data_u8, v_len_u8);
	}
	return com_rslt;
}
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
int8_t bmp280_read_register(uint8_t v_addr_u8, uint8_t *v_data_u8,
		uint8_t v_len_u8)
{
	/* variable used to return communication result*/
	int8_t com_rslt = ERROR;
	/* check the p_bmp280 structure pointer as NULL*/
	if (p_bmp280 == BMP280_NULL) {
		com_rslt = E_BMP280_NULL_PTR;
	} else {
		com_rslt = p_bmp280->BMP280_BUS_READ_FUNC(p_bmp280->dev_addr,
				v_addr_u8, v_data_u8, v_len_u8);
	}
	return com_rslt;
}
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
int8_t bmp280_compute_wait_time(uint8_t *v_delaytime_u8r)
{
	/* variable used to return communication result*/
	int8_t com_rslt = SUCCESS;

	*v_delaytime_u8r = (T_INIT_MAX + T_MEASURE_PER_OSRS_MAX * (((1
			<< p_bmp280->oversamp_temperature)
			>> BMP280_SHIFT_BIT_POSITION_BY_01_BIT)
			+ ((1 << p_bmp280->oversamp_pressure)
			>> BMP280_SHIFT_BIT_POSITION_BY_01_BIT))
			+ ((p_bmp280->oversamp_pressure > 0) ?
			T_SETUP_PRESSURE_MAX : 0) + 15) / 16;
	return com_rslt;
}
