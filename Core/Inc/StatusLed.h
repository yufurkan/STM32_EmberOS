/*
 * StatusLed.h
 *
 *  Created on: Jan 4, 2026
 *      Author: yufur
 */

#ifndef INC_STATUSLED_H_
#define INC_STATUSLED_H_

#include "main.h"

class StatusLed{
public:

	StatusLed(GPIO_TypeDef* port, uint16_t pin);
	~StatusLed();

	void on();
	void off();
	void toggle();
private:

	GPIO_TypeDef* myPort;
	uint16_t      myPin;
};


#endif /* INC_STATUSLED_H_ */
