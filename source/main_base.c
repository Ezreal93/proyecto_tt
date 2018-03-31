#include "hld_spi.h"
#include "hld_uart.h"
#include "hld_gpio.h"
#include "sx1278.h"
#include "serial_stdio.h"
#include "delay.h"

int main(){
    uart1_init(9600);
    serial_printf(uart1_puts, "\n*******\n");
    serial_printf(uart1_puts, "Uart: ok\n");

    serial_printf(uart1_puts, "spi: ok\n");
    hld_pin_ro_t* pin_dio0 = hld_gpio_a0_ro_init();
    hld_pin_ro_t* pin_dio1 = hld_gpio_a1_ro_init();
    hld_pin_wo_t* pin_txen = hld_gpio_a9_wo_init();
    hld_pin_wo_t* pin_rxen = hld_gpio_a10_wo_init();
    hld_pin_wo_t* pin_rst = hld_gpio_b1_wo_init();

    serial_printf(uart1_puts, "gpio: ok\n");

    hld_spi_t* spi =  spi1_init();
    uint8_t data[2] = {0x30,0xff};
    spi->xfer(data, 2, 0, 0);
    
    sx1278_init(spi, pin_dio0, pin_dio1, pin_txen, pin_rxen, pin_rst, BW_500_00_KHZ, SF_12, CR_4_8);
    serial_printf(uart1_puts, "sx1278: ok\n");
    
    while(1){
        /*
        serial_printf(uart1_puts, "sending data:");
        sx1278_tx(buffer, 12);
        serial_printf(uart1_puts, " ok\n");
        delay_ms(1000);
        */
        serial_printf(uart1_puts, "ok asdf;kjijijifjeijejojfaslkdflkjei\n");
        delay_ms(1000);
    }
}