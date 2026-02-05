/*
 * StatusLed.cpp
 *
 *  Created on: Jan 4, 2026
 *      Author: yufur
 */

#include "StatusLed.h"

StatusLed::StatusLed(GPIO_TypeDef* port, uint16_t pin) {
	// TODO Auto-generated constructor stub
	myPort=port;
	myPin=pin;
}

void StatusLed::off(){
	HAL_GPIO_WritePin(myPort,myPin,GPIO_PIN_RESET);
}

void StatusLed::on(){
	HAL_GPIO_WritePin(myPort,myPin,GPIO_PIN_SET);
}

void StatusLed::toggle() {

    HAL_GPIO_TogglePin(myPort, myPin);
}

StatusLed::~StatusLed() {
	// TODO Auto-generated destructor stub
}

