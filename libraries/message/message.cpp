
#include "Arduino.h"
#include "message.h"

//set all bytes of message independantly
void message::set(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, bool g) {

	start = a;
	src = b;
	dest =c;
	cmd = d;
	dat0 = e;
	dat1 = f;
	valid = g;
}

//set all bytes, but dat0, dat1 are produced from a single int16
void message::setInt(uint8_t a, uint8_t b, uint8_t c, uint8_t d, int16_t inData, bool f){

	start = a;
	src = b;
	dest = c;
	cmd = d;
	dat0 = uint8_t(inData >> 8);   //high 1101 0010 1010 1100 >> 0000 0000 1101 0010 = 1101 0010
	dat1 = uint8_t(inData & 0xFF); //low  1101 0010 1010 1100 &  0000 0000 1111 1111 = 1010 1100
	valid = f;
}

//set only dat0, dat1 using a single int16
void message::setDataInt(int inData) {

	dat0 = uint8_t(inData >> 8);   //high 1101 0010 1010 1100 >> 0000 0000 1101 0010 = 1101 0010
	dat1 = uint8_t(inData & 0xFF); //low  1101 0010 1010 1100 &  0000 0000 1111 1111 = 1010 1100
}

//return single int16 from dat0, dat1 
int message::getDataInt() {

	int16_t dat = (dat0 << 8) | dat1; //16 bit int specified to prevent ESP32 from assuming 32 bits (which misinterprets 2's compliment)  
	return dat;
}

//swap source and dest of message
void message::sendBack() {
	uint8_t holdByte = src; //swap source and dest
	src = dest;
	dest = holdByte;
}



