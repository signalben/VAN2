
#ifndef ringbuff_h
#define ringbuff_h
#include <message.h>
#include "Arduino.h"

//External functions from VAN
extern void Elog(uint8_t fileNo, uint8_t errorcode);

class ringbuff {
private:
    Message storage[10]; //Contains the buffered messages
    int readFrom;    //Keeps track of oldest message location (buffer starting point)
    const uint8_t FNUM = 2;//For error reporting, is identification of ackbuff file 

public:
    ringbuff(void);
    bool Available;
    bool Overload;
    int msgCnt;

    void Store(Message);
    Message Read();
    void Show();
};
typedef class ringbuff Ringbuff;
 
#endif