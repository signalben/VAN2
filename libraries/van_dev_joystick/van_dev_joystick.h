//https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/

#ifndef van_dev_joystick_h
#define van_dev_joystick_h
#include "Arduino.h"
#include "message.h"

extern const uint8_t
PERIOD,
REPORT,
SETDEST,
REPORTNOW,
SET;

extern void handleMessage(message inData);
extern void showMessage(message inData);

class van_js {
private:


public:
    uint8_t thisDevice;
    uint8_t destination;
    int velPin;
    int hedPin;
    unsigned long lastReport;
    unsigned int period;

    van_js(uint8_t deviceAdress, int a, int b);
    void command(message inData);
    void autoReport();
    void instantReport();

};
typedef class van_js Van_js;

#endif