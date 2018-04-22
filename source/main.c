#include "hld_i2c.h"
#include "hld_spi.h"
#include "hld_uart.h"
#include "hld_gpio.h"
#include "serial_stdio.h"
#include "sx1278.h"
#include "delay.h"

int main(){
    hld_pin_ro_t* pin_dio0 = hld_gpio_b8_ro_init();
    hld_pin_ro_t* pin_dio1 = hld_gpio_b9_ro_init();
    hld_pin_wo_t* pin_txen = hld_gpio_a2_wo_init();
    hld_pin_wo_t* pin_rxen = hld_gpio_a3_wo_init();
    hld_pin_wo_t* pin_rst = hld_gpio_b14_wo_init_opendrain();
    hld_spi_t* spi =  spi1_init();
  
    sx1278_init(spi, pin_dio0, pin_dio1, pin_txen, pin_rxen, pin_rst, BW_125_00_KHZ, SF_12, CR_4_6);
    char buffer[12] = "Hello world\n";
    while(1){
        sx1278_tx(buffer, 12);
        delay_ms(1000);
    }
}
