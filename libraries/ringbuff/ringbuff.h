
#ifndef ringbuff_h
#define ringbuff_h
#include <message.h>
#include "Arduino.h"

class ringbuff {
private:
    Message storage[10]; //Contains the buffered messages
    int readFrom;    //Keeps track of oldest message location (buffer starting point)

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