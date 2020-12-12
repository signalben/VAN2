
#ifndef message_h
#define message_h
#include "Arduino.h"

struct message {
    
    uint8_t src;
    uint8_t dest;
    uint8_t cmd;
    uint8_t dat0;
    uint8_t dat1;
    bool valid;

	void set(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, bool f);

	void setInt(uint8_t a, uint8_t b, uint8_t c, int inData, bool f);

	void setDataInt(int inData);

    int getDataInt();
};
typedef struct message Message;
   
#endif