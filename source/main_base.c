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
    /*
    hld_spi_t* spi =  spi1_init();
    serial_printf(uart1_puts, "spi: ok\n");
    hld_pin_ro_t* dio0 = gpiob_init(8);
    hld_pin_ro_t* dio1 = gpiob_init(9);
    serial_printf(uart1_puts, "gpio: ok\n");
  
    sx1278_init(spi, dio0, dio1, BW_500_00_KHZ, SF_12, CR_4_8);
    serial_printf(uart1_puts, "sx1278: ok\n");
    
    while(1){
        serial_printf(uart1_puts, "sending data:");
        sx1278_tx(buffer, 12);
        serial_printf(uart1_puts, " ok\n");
        delay_ms(1000);
    }
    */
}

void delay_ms(int delay_time){
    for(int i = 0; i < delay_time; i++){
        for(int j = 0; j < 4000; j++){
        }
    }
}
