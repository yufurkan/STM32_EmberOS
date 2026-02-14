/*
 * Pid.h
 *
 *  Created on: Feb 12, 2026
 *      Author: yufur
 */

#ifndef INC_PID_H_
#define INC_PID_H_


#include <stdint.h>

class Pid {
public:
    // Constructor
    Pid(float kp, float ki, float kd, float minOut, float maxOut);
    // Constructor


    float compute(float setpoint, float input, float dt);

    // Dynamic Adjustment Function (To update according to speed)
    void setTunings(float kp, float ki, float kd);

    // Integral reset
    void reset();

private:
    float _kp, _ki, _kd;
    float _minOut, _maxOut;

    float _prevError;
    float _integral;
};


#endif /* INC_PID_H_ */
