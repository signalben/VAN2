//https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/

#ifndef VAN_US_h
#define VAN_US_h
#include "Arduino.h"
#include "message.h"

extern const uint8_t
ultrasound,
PC,
PERIOD,
REPORT;

extern void handleMessage(message inData);

void CMD_VAN_US(message inData);
void REP_AUTO_VAN_US();
void REP_NOW_VAN_US();

#endif