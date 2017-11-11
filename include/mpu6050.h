#ifndef MPU6050_H
#define MPU6050_H

#include <stdint.h>
#include "hld_api.h"

void mpu6050_init(hld_i2c_t* i2c);
int16_t mpu6050_read_axis(hld_i2c_t* i2c, char axis);
//void readAccelAllAxis(int16_t* ax, int16_t* ay, int16_t* az);

#endif
