/*
 * Compass.h
 *
 *  Created on: Mar 15, 2026
 *      Author: yufur
 */

#ifndef INC_COMPASS_H_
#define INC_COMPASS_H_

#define COMPASS_REG_CONTROL 0x09
#define COMPASS_REG_DATA    0x00
#define COMPASS_I2C_ADDR    (0x2C << 1) // (0x58)

#include "main.h"

class Compass {
public:
    Compass(I2C_HandleTypeDef* i2cHandle);

    bool init();
    void read();
    float getYaw();


private:
    I2C_HandleTypeDef* _i2c;
    float _heading;
    int16_t magX , magY, magZ;

};

#endif /* INC_COMPASS_H_ */
