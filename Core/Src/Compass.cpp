/*
 * Compass.cpp
 *
 *  Created on: Mar 15, 2026
 *      Author: yufur
 */

#include "Compass.h"
#include <math.h>


//#define COMPASS_REG_CONTROL 0x09
//#define COMPASS_REG_DATA    0x00
//#define COMPASS_I2C_ADDR    (0x2C << 1) // (0x58)



Compass::Compass(I2C_HandleTypeDef* i2cHandle) {
	_i2c = i2cHandle;
	magX = 0; magY = 0; magZ = 0;
	_heading = 0.0f;

}

bool Compass::init() {

	uint8_t initData = 0x1D; //<--2C<<1

	HAL_StatusTypeDef status = HAL_I2C_Mem_Write(_i2c, COMPASS_I2C_ADDR, COMPASS_REG_CONTROL, 1, &initData, 1, 100);

	return (status == HAL_OK);
}

void Compass::read() {

	uint8_t buffer[6];
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(_i2c, COMPASS_I2C_ADDR, COMPASS_REG_DATA, 1, buffer, 6, 100);


	if(status==HAL_OK){
		magX=((int16_t)(buffer[0]|(buffer[1])<<8));
		magY=((int16_t)(buffer[2]|(buffer[3])<<8));
		magZ=((int16_t)(buffer[4]|(buffer[5])<<8));

		_heading = atan2((float)magY, (float)magX) * (180.0f / 3.14159265f);
	}



	if (_heading < 0.0f) {
		_heading += 360.0f;
	}


}

float Compass::getYaw() {
    return _heading;
}



