#ifndef van_dev_remote_h
#define van_dev_remote_h
#include "Arduino.h"
#include "message.h"

extern const uint8_t
PERIOD,
REPORT,
REPORTNOW,
SETDEST,
SET;

extern void showMessage(message inData);
extern void handleMessage(message inData);
extern void REM0Update();

class van_rem {
private:

public:
    uint8_t thisDevice;
    uint8_t destination;
    unsigned long lastReport;
    unsigned int period;
    unsigned long IRReadValue;
    //unsigned long previousIRRead;
    //unsigned long left, right, up, down, repeat;

    van_rem(uint8_t deviceAdress);
	void command(message inData);
	void autoReport();
	void instantReport();
};
typedef class van_rem Van_rem;

#endif