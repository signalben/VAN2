//https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/

#ifndef van_dev_imu_h
#define van_dev_imu_h
#include "Arduino.h"
#include "message.h"

extern const uint8_t
PERIOD,
REPORT,
SETDEST,
REPORTNOW,
PARAM0,
PARAM1,
PARAM2,
PARAM3,
PARAM4,
PARAM5;

extern void showMessage(message inData);
extern void handleMessage(message inData);
extern void IMU0Update();

class van_imu {
private:

public:
    uint8_t thisDevice;
    unsigned long lastReport;
    unsigned int period;
    uint8_t AXdest, AYdest, AZdest, ANGXdest, ANGYdest, ANGZdest;
    float AX, AY, AZ, ANGX, ANGY, ANGZ;

    van_imu(uint8_t deviceAdress);
	void command(message inData);
	void autoReport();
	void instantReport();
    void sendChannel(float reading, uint8_t channel, uint8_t destination, int scale);
};
typedef class van_imu Van_imu;

#endif