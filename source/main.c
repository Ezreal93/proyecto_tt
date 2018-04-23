#include "hld_i2c.h"
#include "hld_spi.h"
#include "hld_uart.h"
#include "hld_gpio.h"
#include "serial_stdio.h"
#include "sx1278.h"
#include "delay.h"
#include "bmp280.h"
#include "ms5611.h"

static int32_t temperature;
static uint32_t pressure;

int main(){
    //SX1278 stuff
    hld_pin_ro_t* pin_dio0 = hld_gpio_b8_ro_init();
    hld_pin_ro_t* pin_dio1 = hld_gpio_b9_ro_init();
    hld_pin_wo_t* pin_txen = hld_gpio_a2_wo_init();
    hld_pin_wo_t* pin_rxen = hld_gpio_a3_wo_init();
    hld_pin_wo_t* pin_rst = hld_gpio_b14_wo_init_opendrain();
    hld_spi_t* spi =  spi1_init();
    pin_txen->write(0);
    pin_rxen->write(0);
    //sx1278_init(spi, pin_dio0, pin_dio1, pin_txen, pin_rxen, pin_rst, BW_125_00_KHZ, SF_12, CR_4_6);
    
    //bmp280 stuff
    hld_i2c_t* i2c = i2c1_init();
    ms5611_t ms5611;
    ms5611_i2c_init(&ms5611, i2c, 1);
    uint32_t temp_raw_data;
    uint32_t press_raw_data;
    
    while(1){
        //sx1278_tx(buffer, 12);
        ms5611_adc_start(&ms5611, MS5611_ADC_PRESS, MS5611_OSR_2048);
        delay_ms(100);
        ms5611_adc_read(&ms5611, &press_raw_data);
             delay_ms(100);
        ms5611_adc_start(&ms5611, MS5611_ADC_TEMP, MS5611_OSR_2048);
             delay_ms(100);
        ms5611_adc_read(&ms5611, &temp_raw_data);
             delay_ms(100);
        pressure = ms5611_calc_press(&ms5611, press_raw_data, temp_raw_data, &temperature);
        delay_ms(100);
    }
}
