#include "hld_i2c.h"
#include "hld_spi.h"
#include "hld_uart.h"
#include "serial_stdio.h"
void delay_ms(int delay_time);

int main(){
    uart1_init(9600);
    while(1){
        serial_printf(uart1_puts, "Hello, world!\n");
        delay_ms(1000);
    }
}

void delay_ms(int delay_time){
    for(int i = 0; i < delay_time; i++){
        for(int j = 0; j < 4000; j++){
        }
    }
}
