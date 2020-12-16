
#ifndef ackbuff_h
#define ackbuff_h
#include <message.h>
#include "Arduino.h"

//External constants required from netDef.h, (via main)
extern const uint8_t STD, ACK;
//External functions from VAN
extern void Elog(uint8_t fileNo, uint8_t errorcode);
extern void handleMessage(message inData);
extern void showMessage(message inData);

class ackbuff {
private:
    Message storage[10]; //Contains the buffered messages
    bool messagesEqual(message a, message b);
    bool contains(message inData);
    bool sameFormat(message a, message b);
    const uint8_t FNUM = 1;//For error reporting, is identification of ackbuff file 

public:
    ackbuff(void);
    bool Available;
    bool Overload;
    int msgCnt;
    unsigned long lastReport;
    unsigned int period;

    void add(Message inData);
    void clear(message inData);
    void handleWaiting();
    void clearAll();
    void cancel(Message inData);
    void show(); 
};
typedef class ackbuff Ackbuff;
 
#endif