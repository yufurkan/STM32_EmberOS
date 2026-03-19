/*
 * TelemetryMenager.cpp
 *
 *  Created on: Mar 16, 2026
 *      Author: yufur
 */


#include "TelemetryMenager.h"

TelemetryMenager::TelemetryMenager(UART_HandleTypeDef* huart){
	_huart=huart;
}

void TelemetryMenager::pckData(const ActuatorState_t& commandPack){

	//its done via queue (need write safe mode? )
	_payload.header1=0xAA;
	_payload.header2=0xBB;
	_payload.ch1=commandPack.aileron1_pwm;
	_payload.ch2=commandPack.elevator_pwm;
	_payload.ch3=commandPack.motor_pwm;
	_payload.ch4=commandPack.rudder_pwm;
	_payload.ch5=0;//armed?
	_payload.ch6=commandPack.aileron2_pwm;
	_payload.ch7=0;
	_payload.ch8=0;
	_payload.ch9=0;
	_payload.ch10=0;
	_payload.checksum=calculateChecksum();


}


uint16_t TelemetryMenager::calculateChecksum(){

	uint16_t sum = 0;
	sum += _payload.ch1;
	sum += _payload.ch2;
	sum += _payload.ch3;
	sum += _payload.ch4;
	sum += _payload.ch5;
	sum += _payload.ch6;
	sum += _payload.ch7;
	sum += _payload.ch8;
	sum += _payload.ch9;
	sum += _payload.ch10;

	return sum;
}



void TelemetryMenager::send() {


    HAL_UART_Transmit_DMA(_huart, (uint8_t*)&_payload, sizeof(_payload));
}
