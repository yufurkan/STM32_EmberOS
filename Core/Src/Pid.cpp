#include "Pid.h"

// Constructor
Pid::Pid(float kp, float ki, float kd, float minOut, float maxOut) {
    _kp = kp;
    _ki = ki;
    _kd = kd;
    _minOut = minOut;
    _maxOut = maxOut;
    
    reset();
}

// Updating Coefficients
void Pid::setTunings(float kp, float ki, float kd) {
    if (kp < 0 || ki < 0 || kd < 0) return; // Negatif katsayı olmaz
    _kp = kp;
    _ki = ki;
    _kd = kd;
}

void Pid::reset() {
    _prevError = 0;
    _integral = 0;
}


float Pid::compute(float setpoint, float input, float dt) {
    
    // Error rate
    float error = setpoint - input;

    // Proportional (Oransal)
    float P = _kp * error;

    // Integral (Toplam)
    _integral += (error * dt);


    
    float I = _ki * _integral;
    

    if (I > _maxOut) {
        I = _maxOut; 
        _integral = _maxOut / _ki; // Back-calculation
    } else if (I < _minOut) {
        I = _minOut;
        _integral = _minOut / _ki;
    }


    float derivative = (error - _prevError) / dt;
    float D = _kd * derivative;


    float output = P + I + D;

    if (output > _maxOut) output = _maxOut;
    else if (output < _minOut) output = _minOut;

    // Save error
    _prevError = error;

    return output;
}
