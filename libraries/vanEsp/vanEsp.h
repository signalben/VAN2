//References:
//1.https://techtutorialsx.com/2019/10/20/esp32-getting-started-with-esp-now/
//Ref1 was used to create functions: espNowISR, espNowSetup, and to transmit messages via ESP-NOW in handleMessages

#ifndef vanEsp_h
#define vanEsp_h
#include "Arduino.h"
#include <message.h>
#include <ringbuff.h>
#include <esp_now.h>
#include <WiFi.h>
#include <ackbuff.h>

/*
BOARD SPECIFIC INFO:
vanEsp.h provides a message passing protocol for the ESP32 using three serial ports(0,1,2), and ESP-NOW. Message structure is defined in message.h
For the purpose of routing messages over ESP-NOW to their destination in the same manner as with serial ports, ESP-NOW acts as "serial port 3".
Function espNowSetup() must be called before use.

If the function handleMessage determines a message should be sent via "serial port 3", it instead transmits the message using the ESP-NOW function esp_now_send,
to the MAC address of the other ESP32, defined in netDef.h, which is the other end of "serial port 3".

Upon reception ESP-NOW does not use a hardware buffer to store incoming messages, so an iterupt service routine, espNowISR, must be used
However, it is problematic to act on messages within the ISR: 
Messages from "serial port 3" would have absolute priority over those from other ports, potentially locking them out altogether
If the ISR has not yet completed acting upon a message, subsequent messages recieved via "serial port 3" would be lost

To avoid these problems, the ISR does not act upon a message, it just stores it in a ringBuffer, RB0.Store, then exits.
Messages in RB0 are later checked via getMessage, in the same way it retrieves messages from the hardware serial buffers.

So far, only one bridge can be implimented between two ESP-NOW nodes, potentially "serial ports 4,5,6..." could be added to allow wireless
communication between larger numbers of ESP32 nodes. 

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

//External constants required from netDef.h, (via main)
extern const uint8_t STD, ACK, RESP, THISNODE, ELOGDEST, MSGERROR,
LOCALDEVICE[],
SPORT0[],
SPORT1[],
SPORT2[],
SPORT3[],
N_LOCALDEVICE,
N_SPORT0,
N_SPORT1,
N_SPORT2,
N_SPORT3;
extern uint8_t peerMAC[];//MAC address of other ESP32

extern ackbuff AKB0; //Buffer for storing messages to be repeatedly sent until their correct reception is confirmed (from main) 

//Objects/variables defined in this file
 
const uint8_t FNUM = 3;//For error reporting, is identification of VAN file 

//ESP-NOW functions
void espNowISR(const uint8_t* mac, const uint8_t* data, int len);
void espNowSetup();
//Other functions defined in this file
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
//Functions in main this file calls
void commandList(Message inData);

#endif