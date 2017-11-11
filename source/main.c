#include "hld_i2c.h"
#include "hld_spi.h"
#include "hld_uart.h"

int main(){
  init_i2c1();
  spi1_init();
  uart1_init(9600);
  uart1_puts("Hello, World\n");
  while(1){
  }
}
