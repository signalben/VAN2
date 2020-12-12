//Network Definitions
#ifndef netDef_h
#define netDef_h
#include "Arduino.h"
#include <message.h>

extern const uint8_t
START,
END,
THISNODE,
ULTRASOUND,
JOYSTICK,
IMU,
REMOTE,
MOTORS,
SKIDSTEER,
PID,
SPLITTER,
ROLLOVER,
IMUPID,
PC,
NANO,
NANO_ENC,
MEGA,
ESP32BUGGY,
ESP32STATIC,

LOCALDEVICE[],
SPORT0[],
N_LOCALDEVICE,
N_SPORT0,

REPORT,
PERIOD,
ECHO,
RESPONSE,
SETDEST,
REPORTNOW,
SET,
PARAM0,
PARAM1,
PARAM2,
PARAM3,
PARAM4,
PARAM5;

extern bool
verboseLog,
errorLog;
extern int vanSerial;





#endif
