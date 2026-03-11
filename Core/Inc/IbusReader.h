/*
 * IbusReader.h
 *
 * Created on: Mar 9, 2026
 * Author: yufur
 */

#ifndef INC_IBUSREADER_H_
#define INC_IBUSREADER_H_

#include "SystemManager.h"

#define IBUS_PACKET_SIZE 32
#define IBUS_DMA_BUFFER 64

class IbusReader {

public:

    IbusReader();

    void update(uint8_t* dma_buffer); // parse incoming stream
    RCState getRCState();

    uint16_t getRawChannel(int i){ return _channels[i]; }

private:

    uint16_t _channels[14];
    bool _isDataValid;

    bool validateChecksum(uint8_t* pkt);

    float mapToFloat(uint16_t value, float out_min, float out_max);

    float _out_max;
    float _out_min;
};

#endif
