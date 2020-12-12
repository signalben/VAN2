#include "netDef.h"
#include<vanMega.h>
#include<node.h>
#include <van_dev_motors.h>

van_motors M0(MOTORS,12,3,13,11);

void commandList(Message inData) {
  if (inData.dest == THISNODE) {
    CMD_VAN_NODE(inData);
  }
    if (inData.dest == MOTORS) {
    M0.command(inData);
  }
}

void reportList() {
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
  Serial3.begin(115200);
}

void loop() {
  handleMessage(getMessage(0));
  handleMessage(getMessage(1));
  handleMessage(getMessage(2));
  handleMessage(getMessage(3));
  reportList();
}
