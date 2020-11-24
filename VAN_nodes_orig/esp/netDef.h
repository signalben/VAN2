//Network Definitions
#ifndef netDefMega_h
#define netDefMega_h
#include "Arduino.h"



//Object addresses, must be constant for all nodes: 
uint8_t ultrasound = 0x10;
uint8_t PC = 0x03;
uint8_t nano = 0x04;
uint8_t mega = 0x01;
uint8_t esp32dev = 0x02;


//The following defines all objects in the newtork, relative to this nodes location:

//Objects local to this node (this node, sensors, actuators, functions)
uint8_t lobject[1] = { esp32dev };

//All other object ID's connected via serial port 0
uint8_t port0[0] = {};

//All other object ID's connected via serial port 1
uint8_t port1[0] = {};

//All other object ID's connected via serial port 2
uint8_t port2[3] = { nano, ultrasound, mega };

//All other object ID's connected via serial port 3-- For ESP32 this is WiFi
uint8_t port3[1] = { PC };

//Counts of local objects, and objects reachable on each port 
int n_lobject = 1;
int n_port0 = 0;
int n_port1 = 0;
int n_port2 = 3;
int n_port3 = 1; //For ESP32 this is WiFi

//this node's network map is now defined 

int timer1_counter = 64911;

uint8_t  START = '\a';
uint8_t  END = '\n';
uint8_t message[5];
bool messageValid;

#endif
