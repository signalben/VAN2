//https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/

#ifndef van_dev_motors_h
#define van_dev_motors_h
#include "Arduino.h"
#include "message.h"
#include "ackbuff.h"

extern const uint8_t
PERIOD,
REPORT,
SETDEST,
REPORTNOW,
SET,
PARAM0,
PARAM1,
PARAM2,
PARAM3;

extern ackbuff AKB0;
//External functions from VAN
extern void Elog(uint8_t fileNo, uint8_t errorcode);
extern void handleMessage(message inData);
extern void showMessage(message inData);

class van_motors {
private:


public:
    uint8_t thisDevice;
    uint8_t destination;
    int 
    aDirPin,
    aPwmPin,
    bDirPin,
    bPwmPin; 
    bool safeAhead;
    bool upright;
    unsigned long lastReport;
    unsigned int period;

    van_motors(uint8_t deviceAdress, int a, int b, int c, int d);
    void command(message inData);
    void autoReport();
    void instantReport();
};
typedef class van_motors Van_motors;

#endif