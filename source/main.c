#include "hld_i2c.h"
#include "hld_spi.h"
#include "hld_uart.h"
#include "serial_stdio.h"
#include "mpu6050.h"
#include "bmp280.h"

int main()
    hld_i2c_t* i2c= i2c1_init();
    spi1_init();
    uart1_init(9600);
    serial_printf(uart1_puts, "Hello, World\n");
    mpu6050_init(i2c);
    int16_t datax;

    //bmp280_t bosh;
    //bosh.bus_read = i2c->readPacket;
    //bosh.bus_write = i2c->send_packetAtRegister;
    //bosh.calib_param
    //bosh.dev_addr
    //bosh.delay_msec
    //bosh.oversamp_pressure
    //bosh.oversamp_temperature

    while(1){
        datax = mpu6050_read_axis(i2c, 'x');
        serial_printf(uart1_puts, "dataX: %f\n", (datax/4096.0));
    }
}
