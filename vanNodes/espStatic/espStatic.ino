#include "netDef.h"
#include <vanEsp.h>
#include <node.h>
#include <van_dev_joystick.h>
#include <van_dev_skidsteer.h>

ackbuff AKB0;//Create a buffer to store messages to be repeatedly sent until an acknowledgement is recieved 
//not used on this node, must be created regardless
Van_js JS0(JOYSTICK, 36, 39); //initialize joystick device, with address JOYSTICK, analogue input pins 36, 39 
van_skid SKD0(SKIDSTEER); //initialize skidsteer device, with address SKIDSTEER (used to convert x,y readings from joystick to velocity,heading)

//passes messages with a destination device on this node to that device 
void commandList(Message inData) {
  if (inData.dest == THISNODE) {
    CMD_VAN_NODE(inData);
  }
  if (inData.dest == JOYSTICK) {
    JS0.command(inData);
  }
    if (inData.dest == SKIDSTEER  ) {
    SKD0.command(inData);
  }
}

//devices which may send regular messages (timing handled by each device)
void reportList() {
  JS0.autoReport();
}

void setup() {
  Serial.begin(115200);
  espNowSetup();
  JS0.period = 20; //update joystick readings at 50hz, giving responsive control
  JS0.destination = SKIDSTEER; //converted from x,y readings to velocity,heading
  SKD0.destination = PID; //before being sent as PID target velocities
}

void loop() {
  //nothing but core network functionality, and autoreporting devices
  handleMessage(getMessage(0));
  handleMessage(getMessage(3));
  reportList();
}
