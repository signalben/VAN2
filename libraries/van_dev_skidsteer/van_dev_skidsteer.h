#ifndef van_dev_skidsteer_h
#define van_dev_skidsteer_h
#include "Arduino.h"
#include "message.h"
#include "ackbuff.h"

extern const uint8_t
SET,
SETDEST;

extern ackbuff AKB0;
//External functions from VAN
extern void Elog(uint8_t fileNo, uint8_t errorcode);
extern void handleMessage(message inData);
extern void showMessage(message inData);

class van_skid {
private:


public:
    uint8_t thisDevice;
    uint8_t destination;

    van_skid(uint8_t deviceAdress);
    void command(message inData);
};
typedef class van_skid Van_skid;

#endif