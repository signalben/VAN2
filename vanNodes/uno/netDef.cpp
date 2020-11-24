//Network Definitions
#include "netDef.h"

const uint8_t  START = '\a';
const uint8_t  END = '\0';
//ringbuff RB0;
bool verboseLog = false;
bool errorLog = false;
int vanSerial;

//Object addresses, must be constant for all nodes:
const uint8_t
ultrasound = 16,
PC = 3,
nano = 4,
mega = 1,
esp32buggy = 2,
esp32static = 5;

const uint8_t thisNode = 4;//nano
//address 0x00 not allowed

//The following defines all objects in the newtork, relative to this nodes location:

//Objects local to this node (this node, sensors, actuators, functions)
const uint8_t lobject[2] = { thisNode, ultrasound };

//All other object ID's connected via serial port 0
const uint8_t port0[4] = {PC, mega, esp32buggy, esp32static};

////Counts of local objects, and objects reachable on each port
const uint8_t n_lobject = 2;
const uint8_t n_port0 = 4;

//this node's network map is now defined

//Standard message CMD codes
const uint8_t
REPORT = 1,
PERIOD = 2,
ECHO = 3,
RESPONSE = 4;
