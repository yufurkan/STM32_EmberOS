/*
 * IbusReader.h
 *
 *  Created on: Mar 9, 2026
 *      Author: yufur
 */

#ifndef INC_IBUSREADER_H_
#define INC_IBUSREADER_H_


class IbusReader {
public:

	IbusReader();

	void parse();
	RCState getRCState();

	uint8_t rx_buffer[32];// easy acces for dma, its public

private:

    uint16_t _channels[14];

    bool _isDataValid;
	bool validateChecksum();
	float mapToFloat(uint16_t value, float out_min, float out_max);

	float _out_max;
	float _out_min;
};


#endif /* INC_IBUSREADER_H_ */
