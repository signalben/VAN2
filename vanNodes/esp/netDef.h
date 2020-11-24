//Network Definitions
#ifndef netDef_h
#define netDef_h
#include "Arduino.h"
#include <message.h>
#include <ringbuff.h>
#include <esp_now.h>

uint8_t peerMAC[] = { 0x10, 0x52, 0x1C, 0x68, 0xF3, 0xB0 };
esp_now_peer_info_t peerInfo;
uint8_t  START = '\a';
uint8_t  END = '\0';
ringbuff RB0;
bool verboseLog = false;
bool errorLog = false;
int vanSerial;

//Object addresses, must be constant for all nodes: 
uint8_t ultrasound = 0x10;
uint8_t PC = 0x03;
uint8_t nano = 0x04;
uint8_t mega = 0x01;
uint8_t esp32buggy = 0x02;
uint8_t esp32static = 0x05;
uint8_t dummyobject = 0x42;
//address 0x00 not allowed
uint8_t thisNode = esp32buggy;

//The following defines all objects in the newtork, relative to this nodes location:

//Objects local to this node (this node, sensors, actuators, functions)
uint8_t lobject[1] = { esp32buggy };

//All other object ID's connected via serial port 0
uint8_t port0[1] = {dummyobject};

//All other object ID's connected via serial port 1
uint8_t port1[0] = {};

//All other object ID's connected via serial port 2
uint8_t port2[3] = { nano, ultrasound, mega };

//All other object ID's connected via serial port 3-- For ESP32 this is ESP-NOW 
uint8_t port3[2] = { PC, esp32static };

////Counts of local objects, and objects reachable on each port 
uint8_t n_lobject = 1;
uint8_t n_port0 = 1;
uint8_t n_port1 = 0;
uint8_t n_port2 = 3;
uint8_t n_port3 = 2; //For ESP32 this is ESP-NOW 

//this node's network map is now defined 

#endif
