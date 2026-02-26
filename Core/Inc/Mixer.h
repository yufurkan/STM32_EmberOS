

/*
 * Mixer.h
 *
 *  Created on: Feb 25, 2026
 *      Author: yufur
 */

#ifndef INC_MIXER_H_
#define INC_MIXER_H_


#include "SystemManager.h"


struct ActuatorState_t{
	uint16_t motor_pwm;//1000-2000
	uint16_t aileron1_pwm;
	uint16_t aileron2_pwm;
	uint16_t elevator_pwm;
	uint16_t rudder_pwm;
};


class ActuatorMixer{


	public:

		ActuatorMixer(float minpid_servo,float maxpid_servo,float _minpid_motor,float _maxpid_motor);//constructor

		uint16_t mapToPWM(float value, float in_min, float in_max, uint16_t out_min, uint16_t out_max);

		void compute( RCState infos, PIDOuts pid_infos);

		ActuatorState_t getState();

	private:

		ActuatorState_t _pwmOutputs;


		//for now motor max pid are same for servos but if will be needed It change to struct holding evry servo max min values and add function for cahnge them
		float _maxpid_servo;
		float _minpid_servo;
		float _minpid_motor;
		float _maxpid_motor;
};

#endif /* INC_MIXER_H_ */

