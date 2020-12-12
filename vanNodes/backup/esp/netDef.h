//Network Definitions
#ifndef netDef_h
#define netDef_h
#include "Arduino.h"
#include <message.h>
#include <esp_now.h>

extern uint8_t peerMAC[];
extern esp_now_peer_info_t peerInfo;

extern const uint8_t
START,
END,
ULTRASOUND,
PC,
NANO,
MEGA,
ESP32BUGGY,
ESP32STATIC,
THISNODE,

LOCALDEVICE[],
SPORT0[],
SPORT1[],
SPORT2[],
SPORT3[],
N_LOCALDEVICE,
N_SPORT0,
N_SPORT1,
N_SPORT2,
N_SPORT3,

REPORT,
PERIOD,
ECHO,
RESPONSE,
SETDEST,
REPORTNOW;

extern bool
verboseLog,
errorLog;
extern int vanSerial;





#endif
