#include "hld_i2c.h"
#include "hld_spi.h"
#include "hld_uart.h"
#include "serial_stdio.h"
#include "mpu6050.h"

int main(){
  init_i2c1();
  spi1_init();
  uart1_init(9600);
  serial_printf(uart1_puts, "Hello, World %d\n", 10);
  while(1){
  }
}
