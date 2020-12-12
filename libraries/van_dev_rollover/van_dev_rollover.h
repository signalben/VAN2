#ifndef van_dev_rollover_h
#define van_dev_rollover_h
#include "Arduino.h"
#include "message.h"

extern const uint8_t
SETDEST,
PARAM1,
PARAM4;

extern void showMessage(message inData);
extern void handleMessage(message inData);

class van_roll {
private:


public:
    uint8_t thisDevice;
    uint8_t destination;
    int angle;

    van_roll(uint8_t deviceAdress);
    void command(message inData);
    void assessDanger(int angle);
};
typedef class van_roll Van_roll;

#endif