//Network Definitions
#ifndef netDef_h
#define netDef_h
#include "Arduino.h"
#include <message.h>

extern const uint8_t
START,
END,
ultrasound,
PC,
nano,
mega,
esp32buggy,
esp32static,
thisNode,

lobject[],
port0[],
n_lobject,
n_port0,

REPORT,
PERIOD,
ECHO,
RESPONSE;

extern bool
verboseLog,
errorLog;
extern int vanSerial;





#endif
