/*
 * IbusReader.cpp
 *
 * Created on: Mar 9, 2026
 * Author: yufur
 */



#include "IbusReader.h"
#include <string.h>

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

bool IbusReader::validateChecksum(uint8_t* pkt){
	uint16_t checksum = 0xFFFF;

	for (int i = 0; i < 30; i++) {
		checksum -= pkt[i];
	}

	uint16_t rxsum  = (pkt[31]<<8) | pkt[30];

    return (checksum==rxsum);
}


void IbusReader::update(uint8_t* buffer){

    // search packet header inside dma buffer
	for(int i = 0; i <= IBUS_DMA_BUFFER - IBUS_PACKET_SIZE; i++)
	{
	    if(buffer[i] == 0x20 && buffer[i+1] == 0x40)
	    {
	        uint8_t pkt[IBUS_PACKET_SIZE];
	        memcpy(pkt, &buffer[i], IBUS_PACKET_SIZE);

	        if(validateChecksum(pkt))
	        {
	            _isDataValid = true;

	            for(int ch=0; ch<14; ch++)
	            {
	                int idx = 2 + ch*2;
	                _channels[ch] = pkt[idx] | (pkt[idx+1]<<8);
	            }

	            return;
	        }
	    }
	}
}



float IbusReader::mapToFloat(uint16_t value, float out_min, float out_max) {


    if(value<1000) value=1000;
    if(value>2000) value=2000;

	// Linear Mapping
	float mapped = ((float)(value - 1000) / 1000.0f) * (out_max - out_min) + out_min;
	return mapped;
}

RCState IbusReader::getRCState() {

	// NOTE: In Mixer.cpp/h ailerons set for ch1(left) and ch6(right). ch6 setted for opposite of ch1 in Mixer file but actually ı noticed no need this I can extract the cahnnel information from transmitter and I can regulate the aileron behavior from the remote control.
	// This option is on hold for now; I'll consider it later.

	RCState state;

	if(!_isDataValid)
	{
		state.armed=false;
		state.roll=0;
		state.pitch=0;
		state.yaw=0;
		state.throttle=0;
		return state;
	}

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
