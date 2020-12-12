//https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/

#ifndef van_dev_ultrasound_h
#define van_dev_ultrasound_h
#include "Arduino.h"
#include "message.h"

extern const uint8_t
PERIOD,
REPORT,
SETDEST,
REPORTNOW,
MOTORS,
SET,
PID,
PARAM0,
PARAM1,
PARAM2,
PARAM3;

extern void handleMessage(message inData);

class van_us {
private:


public:
    uint8_t thisDevice;
    uint8_t destination;
    int trigPin;
    int echoPin;
    int obstructed;
    unsigned long lastReport;
    unsigned int period;

    van_us(uint8_t deviceAdress, int a, int b);
    void command(message inData);
    void autoReport();
    void instantReport();
};
typedef class van_us Van_us;

#endif