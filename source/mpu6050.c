#include "mpu6050.h"

#define MPU6050_ADDRESS 0x68

#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_ACCEL_XOUT_L 0x3C
#define MPU6050_ACCEL_YOUT_H 0x3D
#define MPU6050_ACCEL_YOUT_L 0x3E
#define MPU6050_ACCEL_ZOUT_H 0x3F
#define MPU6050_ACCEL_ZOUT_L 0x40

#define MPU6050_GYRO_XOUT_H 0x43
#define MPU6050_GYRO_XOUT_L 0x44
#define MPU6050_GYRO_YOUT_H 0x45
#define MPU6050_GYRO_YOUT_L 0x46
#define MPU6050_GYRO_ZOUT_H 0x37
#define MPU6050_GYRO_ZOUT_L 0x48


#define MPU6050_AFSEL_2G 0
#define MPU6050_AFSEL_4G 1
#define MPU6050_AFSEL_8G 2
#define MPU6050_AFSEL_16G 3
#define MPU6050_AFSEL_POS 3
#define MPU6050_ACCEL_CONFIG 0x1C

static void writeRegister(hld_i2c_t* i2c, uint8_t regAddress, uint8_t regValue);

void mpu6050_init(hld_i2c_t* i2c){
    writeRegister(i2c, 0x6B, 0x01);// reset
    writeRegister(i2c, 0x6B, 0x00);//
    writeRegister(i2c, 0x1A, 0x06);// 5Hz filter
    writeRegister(i2c, MPU6050_ACCEL_CONFIG, (MPU6050_AFSEL_8G << MPU6050_AFSEL_POS ));
    writeRegister(i2c, 0x1B, (0x02 << 3));// +- 1000 grads/s
}

static void writeRegister(hld_i2c_t* i2c, uint8_t regAddress, uint8_t regValue){
    uint8_t data_packet[2] = {regAddress,regValue};
    i2c->sendPacket(MPU6050_ADDRESS,data_packet,2);
}

int16_t mpu6050_read_axis(hld_i2c_t* i2c, char axis){
    int16_t accel_reading = 0;
    uint8_t axis_data_h;
    uint8_t axis_data_l;
    uint8_t axis_address_h;

    if((axis >= 'x') && (axis <= 'z')){
        axis_address_h = MPU6050_ACCEL_XOUT_H + (axis - 'x')*2;
        axis_data_h = i2c->readData(MPU6050_ADDRESS,axis_address_h);
        axis_data_l = i2c->readData(MPU6050_ADDRESS,axis_address_h+1);
        accel_reading =(int16_t) (axis_data_h << 8) | (axis_data_l);
    }
    return accel_reading;
}

/*
int16_t MPU6050::readGyro(char axis){
    int16_t gyro_reading = 0;
    unsigned char axis_data_h;
    unsigned char axis_data_l;
    unsigned char axis_address_h;

    if((axis >= 'x') && (axis <= 'z')){
        axis_address_h = MPU6050_GYRO_XOUT_H + (axis - 'x')*2;
        axis_data_h = i2c->readData(MPU6050_ADDRESS,axis_address_h);
        axis_data_l = i2c->readData(MPU6050_ADDRESS,axis_address_h+1);
        gyro_reading = (axis_data_h << 8) | (axis_data_l);
    }
    return gyro_reading;
}

void MPU6050::readAccelAllAxis(int16_t* ax, int16_t* ay, int16_t* az){
    unsigned char accel_data[6];
    i2c->readPacket(MPU6050_ADDRESS,MPU6050_ACCEL_XOUT_H,accel_data,6);
    *ax = (accel_data[0] << 8) | (accel_data[1]);
    *ay = (accel_data[2] << 8) | (accel_data[3]);
    *az = (accel_data[4] << 8) | (accel_data[5]);
}

void MPU6050::readGyroAllAxis(int16_t* gx, int16_t* gy, int16_t* gz){
    unsigned char gyro_data[6];
    i2c->readPacket(MPU6050_ADDRESS,MPU6050_GYRO_XOUT_H,gyro_data,6);
    *gx = (gyro_data[0] << 8) | (gyro_data[1]);
    *gy = (gyro_data[2] << 8) | (gyro_data[3]);
    *gz = (gyro_data[4] << 8) | (gyro_data[5]);
}

Data3D<float> MPU6050::readCompensatedGyro(void){
    int16_t gx, gy, gz;
    float scale = this->scaleRange * (2.0/65535.0);
    readGyroAllAxis(&gx, &gy, &gz);

    gx -= offset_x;
    gy -= offset_y;
    gz -= offset_z;

    return Data3D<float>((gx * scale),(gy * scale),(gz * scale));
}

void MPU6050::calibrateGyro(void){
    int readings = 500;
    int16_t gx, gy, gz;
    int sum_x = 0;
    int sum_y = 0;
    int sum_z = 0;
    for(int i = 0;  i < readings; i++){
        readGyroAllAxis(&gx, &gy, &gz);
        sum_x += gx;
        sum_y += gy;
        sum_z += gz;
    }
    this->offset_x = sum_x/readings;
    this->offset_y = sum_y/readings;
    this->offset_z = sum_z/readings;
}
*/
