#include "netDef.h"
#include <vanEsp.h>
#include <node.h>
#include <van_dev_joystick.h>
#include <van_dev_skidsteer.h>

ackbuff AKB0;//Create a buffer to store messages to be repeatedly sent until an acknowledgement is recieved 
Van_js JS0(JOYSTICK, 36, 39);
van_skid SKD0(SKIDSTEER);

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

void reportList() {
  JS0.autoReport();
}

void setup() {
  Serial.begin(115200);
  espNowSetup();
  JS0.period = 20;
  JS0.destination = SKIDSTEER;
  SKD0.destination = PID;
}

void loop() {
  handleMessage(getMessage(0));
  handleMessage(getMessage(3));
  reportList();
}
