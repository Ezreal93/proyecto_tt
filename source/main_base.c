#include "hld_spi.h"
#include "hld_uart.h"
#include "hld_gpio.h"
#include "sx1278.h"
#include "serial_stdio.h"
void delay_ms(int delay_time);

int main(){
    uart1_init(9600);
    serial_printf(uart1_puts, "\n*******\n");
    serial_printf(uart1_puts, "Uart: ok\n");

    serial_printf(uart1_puts, "spi: ok\n");
    hld_pin_ro_t* pa0 = hld_gpio_a0_ro_init();
    hld_pin_ro_t* pa1 = hld_gpio_a1_ro_init();
    hld_pin_wo_t* pa9 = hld_gpio_a9_wo_init();
    hld_pin_wo_t* pa10 = hld_gpio_a10_wo_init();
    serial_printf(uart1_puts, "gpio: ok\n");
    int8_t state = 0;

    /*
    hld_spi_t* spi =  spi1_init();
    sx1278_init(spi, dio0, dio1, BW_500_00_KHZ, SF_12, CR_4_8);
    serial_printf(uart1_puts, "sx1278: ok\n");
    */

    while(1){
        /*
        serial_printf(uart1_puts, "sending data:");
        sx1278_tx(buffer, 12);
        serial_printf(uart1_puts, " ok\n");
        delay_ms(1000);
        */
        state = pa0->read();
        pa9->write(state);
        state = pa1->read();
        pa10->write(state);
    }
}

void delay_ms(int delay_time){
    for(int i = 0; i < delay_time; i++){
        for(int j = 0; j < 4000; j++){
        }
    }
}
