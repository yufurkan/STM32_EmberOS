/*
 * IbusReader.cpp
 *
 * Created on: Mar 9, 2026
 * Author: yufur
 */



#include "IbusReader.h"

IbusReader::IbusReader(){
	_isDataValid = false;

	// Security: All channels mid (1500)
	for(int i=0; i<14; i++) {
	    _channels[i] = 1500;
	}
	_channels[2] = 1000; // CH3 (Motor) closed
	_channels[4] = 1000;

	_out_max=20.0f;
	_out_min=-20.0f;
}

bool IbusReader::validateChecksum() {
	uint16_t checksum = 0xFFFF;

	for (int i = 0; i < 30; i++) {
		checksum -= rx_buffer[i];
	}

	uint16_t checksum2 = (rx_buffer[31]<<8) | rx_buffer[30];

	if(checksum2==checksum)return true; else return false;
}

void IbusReader::parse() {
	// The packet must start with 0x20 (Length) and continue with 0x40 (Command)
	if (rx_buffer[0] == 0x20 && rx_buffer[1] == 0x40) {

		if (validateChecksum()) {
			_isDataValid = true;

			for(int i=0; i<14;i++){
				int index = 2 + (i * 2);
				_channels[i] = (rx_buffer[index+1]<<8)|(rx_buffer[index]);
			}

		} else {
			_isDataValid = false;
		}
	} else {
		_isDataValid = false;
	}
}

float IbusReader::mapToFloat(uint16_t value, float out_min, float out_max) {


	if (value < 1000) value = 1000;
	if (value > 2000) value = 2000;

	// Linear Mapping
	float mapped = ((float)(value - 1000) / 1000.0f) * (out_max - out_min) + out_min;
	return mapped;
}

RCState IbusReader::getRCState() {

	// NOTE: In Mixer.cpp/h ailerons set for ch1(left) and ch6(right). ch6 setted for opposite of ch1 in Mixer file but actually ı noticed no need this I can extract the cahnnel information from transmitter and I can regulate the aileron behavior from the remote control.
	// This option is on hold for now; I'll consider it later.

	RCState state;

	if(_channels[4]>1500){
		state.armed = true; //armed state CH5
		state.roll = mapToFloat(_channels[0], _out_min, _out_max); //ailerons CH1
		state.pitch = mapToFloat(_channels[1], _out_min, _out_max); //elevator CH2
		state.throttle = mapToFloat(_channels[2], 0.0f, 100.0f); //motor CH3
		state.yaw = mapToFloat(_channels[3], _out_min, _out_max); //rudder CH4
	}else{
		state.armed = false;
		state.roll = 0.0f;
		state.pitch = 0.0f;
		state.yaw = 0.0f;
		state.throttle = 0.0f;
	}

	return state;
}
