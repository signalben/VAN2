
#ifndef node_h
#define node_h
#include "Arduino.h"
#include "message.h"

void CMD_VAN_NODE(Message inData);
void echoMessage(Message inData);

extern const uint8_t
ECHO,
RESPONSE;
extern void handleMessage(Message inData);
   
#endif