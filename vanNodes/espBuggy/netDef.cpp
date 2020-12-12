//Network Definitions
#include "netDef.h"

  //Get via Serial.println(WiFi.macAddress());
  //This ESP is buggy (old): {0xFC, 0xF5, 0xC4, 0x0F, 0xBE, 0xFC};
  //Other ESP is static (new): MAC{0x10, 0x52, 0x1C, 0x68, 0xF3, 0xB0}; 

uint8_t peerMAC[] = {0x10, 0x52, 0x1C, 0x68, 0xF3, 0xB0}; //Adress of other esp
esp_now_peer_info_t peerInfo;

bool verboseLog = false;
bool errorLog = false;
int vanSerial;

const uint8_t  
START = '\a',
END = '\0';

//Device addresses, must be constant for all nodes:
const uint8_t
ULTRASOUND = 16,
JOYSTICK = 17,
IMU = 18,
REMOTE = 19,
MOTORS = 20,
SKIDSTEER = 30,
PID = 31,
SPLITTER = 32,
ROLLOVER = 33,
IMUPID = 34,
PC = 3,
NANO = 4,
NANO_ENC = 6,
MEGA = 1,
ESP32BUGGY = 2,
ESP32STATIC = 5;

const uint8_t THISNODE = 2;//esp onboard initio buggy
//address 0x00 not allowed

//The following defines all objects in the newtork, relative to this nodes location:

//Devices local to this node (this node, sensors, actuators, functions)
const uint8_t LOCALDEVICE[5] = {THISNODE, IMU, SPLITTER, IMUPID, ROLLOVER};

//All other device ID's connected via serial port 0
const uint8_t SPORT0[0] = {};
const uint8_t SPORT1[0] = {};
const uint8_t SPORT2[6] = {MEGA, MOTORS, NANO, ULTRASOUND, NANO_ENC, PID};
const uint8_t SPORT3[3] = {ESP32STATIC, SKIDSTEER,PC};

////Counts of local devices, and devices reachable on each serial port
const uint8_t 
N_LOCALDEVICE = 5,
N_SPORT0 = 0,
N_SPORT1 = 0,
N_SPORT2 = 6,
N_SPORT3 = 3;

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
PARAM5 = 13;
