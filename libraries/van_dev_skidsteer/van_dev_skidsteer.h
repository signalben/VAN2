//https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/

#ifndef van_dev_skidsteer_h
#define van_dev_skidsteer_h
#include "Arduino.h"
#include "message.h"

extern const uint8_t
SET,
SETDEST;

extern void showMessage(message inData);
extern void handleMessage(message inData);

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