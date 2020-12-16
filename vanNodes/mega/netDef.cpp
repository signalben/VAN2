//Network Definitions
#include "netDef.h"

const uint8_t  
STD = '\a',
ACK = 90,
RESP = 110,
ELOGDEST = 0; //Elog not active

//Device addresses, must be constant for all nodes:
const uint8_t
ULTRASOUND = 16,
JOYSTICK = 17,
IMU = 18,
REMOTE = 19,
MOTORS = 20,
SKIDSTEER = 30,
PID = 31,
ROLLOVER = 33,
PC = 3,
NANO = 4,
NANO_ENC = 6,
MEGA = 1,
ESP32BUGGY = 2,
ESP32STATIC = 5;

const uint8_t THISNODE = 1;//This is the mega, (on buggy)
//address 0x00 not allowed

//The following defines all objects in the newtork, relative to this nodes location:

//Devices local to this node (this node, sensors, actuators, functions)
const uint8_t LOCALDEVICE[2] = {THISNODE, MOTORS};

//All other device ID's connected via serial port 0
const uint8_t SPORT0[0] = {};
const uint8_t SPORT1[6] = {PC,ESP32BUGGY,IMU, ROLLOVER, ESP32STATIC, SKIDSTEER};
const uint8_t SPORT2[2] = {NANO, ULTRASOUND};
const uint8_t SPORT3[2] = {NANO_ENC,PID};

////Counts of local devices, and devices reachable on each port
const uint8_t 
N_LOCALDEVICE = 2,
N_SPORT0 = 0,
N_SPORT1 = 6,
N_SPORT2 = 2,
N_SPORT3 = 2;

//this node's network map is now defined

//Standard message CMD codes
const uint8_t
REPORT = 1,
PERIOD = 2,
ECHO = 3,
RESPONSE = 4,
SETDEST = 5,
REPORTNOW = 6,
SET = 7,
PARAM0 = 8,
PARAM1 = 9,
PARAM2 = 10,
PARAM3 = 11,
PARAM4 = 12,
PARAM5 = 13,
MSGERROR = 14;
