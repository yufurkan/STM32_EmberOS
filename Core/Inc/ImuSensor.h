/*
 * ImuSensor.h
 *
 *  Created on: Jan 18, 2026
 *      Author: yufur
 */

#ifndef SRC_IMUSENSOR_H_
#define SRC_IMUSENSOR_H_

#include "main.h"
#include <stdint.h>

struct MPU_DATA{
	float AccX;
	float AccY;
	float AccZ;
	float gyX;
	float gyY;
	float gyZ;
	float totalforce;
};


class ImuSensor {
public:
	ImuSensor(I2C_HandleTypeDef* hi2c);

	bool init();
	void readAccel();
	void readGyro();
	MPU_DATA getData();


private:

	MPU_DATA mpu_data;
	I2C_HandleTypeDef* _i2c;
	HAL_StatusTypeDef mpu_status;


};

#endif /* SRC_IMUSENSOR_H_ */
