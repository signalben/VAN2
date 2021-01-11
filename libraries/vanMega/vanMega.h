#ifndef vanMega_h
#define vanMega_h

#include "Arduino.h"
#include <message.h>
#include <ackbuff.h>
/*
BOARD SPECIFIC INFO:
vanUno.h provides a message passing protocol for arduinos with up to 4 serial ports (such as mega) and no wifi capabilities, Message structure is defined in message.h

GENERAL INFO:
This file does not need to be modified to define a network, but does require a network map from netDef.h to operate.
This a connectionless protocol, when using STD messages, source devices are unaware of whether the destination device has recieved the message.
A device may send a single message to another using handleMessage(message), regardless of whether the source/destination devices share the same parent node or not.
Recieved and internal messages which are addressed to a device on this node are passed to the function commandList(message), in main loop, which in turn
passes them to the device itself.

An acknowledgment protocol can also be used, where instead the device adds the STD message to a buffer, AKB0.add(message)
This message will then be repeated as an ACK message until either:
A response, RESP message, from the destination node veryifies it was recieved correctly
The sending device cancels the message due to a change in conditions making the original message obsolete, using AKB0.cancel(message)

The acknowledgment protocol does not gaurentee that the recipient device has got the message, or acted upon it
It only garentees that the recipient device's parent node has recieved it correctly
i.e It is intended to correct communication errors, not software errors

An XOR based checksum is used when transmitting/recieving, corrupt messages are dropped
Error codes can be sent via the nework to a destination specified in netDef.h

Function showMessage(message) provides a more human-freindly display of messages on a serial monitor, but is interpreted by recieving nodes as junk
i.e It is intended for debugging, not for use in a working system
*/

extern const uint8_t STD, ACK, RESP, THISNODE, ELOGDEST, MSGERROR, STOREDACK,
LOCALDEVICE[], SPORT0[], SPORT1[], SPORT2[], SPORT3[],
N_LOCALDEVICE, N_SPORT0, N_SPORT1, N_SPORT2, N_SPORT3;

extern ackbuff AKB0; //Buffer for storing messages to be repeatedly sent until their correct reception is confirmed (from main)
const uint8_t FNUM = 3;//For error reporting, is identification of VAN file 
extern int corrupted; //count of received messages which fail checksum, used for testing 

void Elog(uint8_t inData);
void vanWrite(uint8_t outbyte);
int vanAvailable();
uint8_t vanRead();
uint8_t getStart();
uint8_t calcChecksum(message inData);
Message getMessage(int selected);
void handleMessage(Message inData);
void sendMessage(Message inData);
void showMessage(Message inData);
void commandList(Message inData);


#endif