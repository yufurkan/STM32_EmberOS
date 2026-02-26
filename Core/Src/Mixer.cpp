

/*
 * Mixer.cpp
 *
 *  Created on: Feb 25, 2026
 *      Author: yufur
 */

#include "Mixer.h"


ActuatorMixer::ActuatorMixer(float minpid_servo,float maxpid_servo,float minpid_motor,float maxpid_motor){

	 _maxpid_servo=maxpid_servo;
	 _minpid_servo=minpid_servo;
	 _minpid_motor=minpid_motor;
	 _maxpid_motor=maxpid_motor;
}


uint16_t ActuatorMixer::mapToPWM(float value, float in_min, float in_max, uint16_t out_min, uint16_t out_max) {
    // linear mapping equation
    float mapped = (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

    // constrain to limits for safety
    if (mapped > out_max) mapped = out_max;
    if (mapped < out_min) mapped = out_min;

    return (uint16_t)mapped;
}


void ActuatorMixer::compute( RCState infos, PIDOuts pid_infos){


	if(infos.armed){
		_pwmOutputs.aileron1_pwm = mapToPWM(pid_infos.servo_roll_out, _minpid_servo, _maxpid_servo, 1000, 2000);
		_pwmOutputs.aileron2_pwm = mapToPWM(-pid_infos.servo_roll_out, _minpid_servo, _maxpid_servo, 1000, 2000); // reverse
		_pwmOutputs.elevator_pwm = mapToPWM(pid_infos.servo_pitch_out, _minpid_servo, _maxpid_servo, 1000, 2000);
		_pwmOutputs.rudder_pwm   = mapToPWM(pid_infos.servo_yaw_out, _minpid_servo, _maxpid_servo, 1000, 2000);

		_pwmOutputs.motor_pwm = mapToPWM(infos.throttle, _minpid_motor, _maxpid_motor, 1000, 2000);
	}else {
        // disarmed state
        _pwmOutputs.aileron1_pwm = 1500; // center
        _pwmOutputs.aileron2_pwm = 1500;
        _pwmOutputs.elevator_pwm = 1500;
        _pwmOutputs.rudder_pwm   = 1500;
        _pwmOutputs.motor_pwm    = 1000; // motor off
    }


}

ActuatorState_t ActuatorMixer::getState(){
	return _pwmOutputs;
}


