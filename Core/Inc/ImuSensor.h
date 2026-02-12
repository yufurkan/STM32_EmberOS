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

	float totalforce;

	float gyX;
	float gyY;
	float gyZ;

	float pitch;
	float roll;
	float yaw;//accel cant messure yaw angle information confirmation is missing.
};





class ImuSensor {
public:
	ImuSensor(I2C_HandleTypeDef* hi2c);

	bool init();
	void readAccel();
	void readGyro();
	MPU_DATA getData();
	void angleMeasurement();

private:

	//Variables for angular estimation and time measurement
	uint32_t _prevTime;

	float _alpha;
	float _dt;
	float _compAngle=0;

	//ANGLEDATA angle_data;

	//sensor read variables
	MPU_DATA mpu_data;
	I2C_HandleTypeDef* _i2c;
	HAL_StatusTypeDef mpu_status;


};

#endif /* SRC_IMUSENSOR_H_ */
