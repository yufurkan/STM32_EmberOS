/*
 * ImuSensor.cpp
 *
 *  Created on: Jan 18, 2026
 *      Author: yufur
 */

#include "ImuSensor.h"
#include <math.h>

#define MPU6050_ADDR         0xD0 // (0x68 << 1)
#define PWR_MGMT_1_REG       0x6B // wake point
#define ACCEL_XOUT_H_REG     0x3B // sector that we read values
#define INT_PIN_CFG          0x37 //register adress for open QMC5883L
#define GYRO_XOUT_H			 0x43 //QMC5883L Gyro start adress


ImuSensor::ImuSensor(I2C_HandleTypeDef *hi2c) {
	// TODO Auto-generated constructor stub
	_i2c=hi2c;
}

bool ImuSensor::init() {
	uint8_t uyandirici = 0;


	mpu_status=HAL_I2C_Mem_Write(_i2c,MPU6050_ADDR,PWR_MGMT_1_REG,1, &uyandirici,1,100);

	if(mpu_status != HAL_OK)return 0;

	uyandirici = 0x02;
	mpu_status = HAL_I2C_Mem_Write(_i2c, MPU6050_ADDR, INT_PIN_CFG, 1, &uyandirici, 1, 100);
	if(mpu_status == HAL_OK)return 1;
	return 0;
}



void ImuSensor::readAccel(){
	uint8_t buffer[6];

	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(_i2c, MPU6050_ADDR, ACCEL_XOUT_H_REG, 1, buffer, 6, 100);

	if(status==HAL_OK){
		mpu_data.AccX=((float)((int16_t)(buffer[0] << 8 | buffer[1]))/16384.0);
		mpu_data.AccY=((float)((int16_t)(buffer[2] << 8 | buffer[3]))/16384.0);
		mpu_data.AccZ=((float)((int16_t)(buffer[4] << 8 | buffer[5]))/16384.0);

		if(mpu_data.AccX==0 && mpu_data.AccY==0 && mpu_data.AccZ==0)return ; // add i2c line reboot

		mpu_data.totalforce = sqrt((mpu_data.AccX * mpu_data.AccX) + (mpu_data.AccY * mpu_data.AccY) + (mpu_data.AccZ * mpu_data.AccZ));

	}

}

MPU_DATA ImuSensor::getData() {
    return mpu_data;
}

void ImuSensor::readGyro(){
	uint8_t buffer2[6];

	// using MPU6050_ADDR for read gyro
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(_i2c, MPU6050_ADDR, GYRO_XOUT_H, 1, buffer2, 6, 100);

	if(status==HAL_OK){
			mpu_data.gyX=((float)((int16_t)(buffer2[0] << 8 | buffer2[1]))/131.0);
			mpu_data.gyY=((float)((int16_t)(buffer2[2] << 8 | buffer2[3]))/131.0);
			mpu_data.gyZ=((float)((int16_t)(buffer2[4] << 8 | buffer2[5]))/131.0);
	}

}

