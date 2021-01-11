//Network Definitions
#ifndef netDef_h
#define netDef_h
#include "Arduino.h"
#include <message.h>

extern const uint8_t
//Message passing constants
STD,
ACK,
RESP,
ELOGDEST,

//Device addresses, must be constant across all nodes:
THISNODE,
ULTRASOUND,
JOYSTICK,
IMU,
REMOTE,
MOTORS,
SKIDSTEER,
PID,
ROLLOVER,
PC,
NANO,
NANO_ENC,
MEGA,
ESP32BUGGY,
ESP32STATIC,

//Devices local to this node (this node, sensors, actuators, functions)
LOCALDEVICE[],
//All other device ID's connected via serial ports 
SPORT0[],
SPORT1[],
SPORT2[],
SPORT3[],

//Counts of local devices, and devices reachable on each serial port
N_LOCALDEVICE,
N_SPORT0,
N_SPORT1,
N_SPORT2,
N_SPORT3,

//Standard message CMD codes
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
PARAM5,
MSGERROR;

#endif
