/*
 * TelemetryMenager.h
 *
 *  Created on: Mar 16, 2026
 *      Author: yufur
 */

#ifndef INC_TELEMETRYMENAGER_H_
#define INC_TELEMETRYMENAGER_H_

#include "main.h"
#include "Mixer.h"

#pragma pack(push, 1)

struct Pack{

	uint8_t header1;
	uint8_t header2;
	int16_t ch1;
	int16_t ch2;
	int16_t ch3;
	int16_t ch4;
	int16_t ch5;
	int16_t ch6;
	int16_t ch7;
	int16_t ch8;
	int16_t ch9;
	int16_t ch10;
	uint16_t checksum;
};

#pragma pack(pop)


class TelemetryMenager{
public:

	TelemetryMenager(UART_HandleTypeDef* huart);

	void pckData(const ActuatorState_t& commandPack);

	void send();


private:

	UART_HandleTypeDef* _huart;
	Pack _payload;
	uint16_t calculateChecksum();
};

#endif /* INC_TELEMETRYMENAGER_H_ */
