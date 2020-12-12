
#include "Arduino.h"
#include "message.h"

void message::set(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, bool f) {

	src = a;
	dest = b;
	cmd = c;
	dat0 = d;
	dat1 = e;
	valid = f;
}

void message::setInt(uint8_t a, uint8_t b, uint8_t c, int inData, bool f) {

	src = a;
	dest = b;
	cmd = c;
	dat0 = uint8_t(inData >> 8);   //high 1101 0010 1010 1100 >> 0000 0000 1101 0010 = 1101 0010
	dat1 = uint8_t(inData & 0xFF); //low  1101 0010 1010 1100 &  0000 0000 1111 1111 = 1010 1100
	valid = f;
}

void message::setDataInt(int inData) {

	dat0 = uint8_t(inData >> 8);   //high 1101 0010 1010 1100 >> 0000 0000 1101 0010 = 1101 0010
	dat1 = uint8_t(inData & 0xFF); //low  1101 0010 1010 1100 &  0000 0000 1111 1111 = 1010 1100
}

int message::getDataInt() {

	int16_t dat = (dat0 << 8) | dat1; //16 bit int specified to prevent ESP32 from assuming 32 bits (which misinterprets 2's compliment)  
	return dat;
}



