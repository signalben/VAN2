#include "netDef.h"
#include<vanUno.h>
#include<node.h>
#include<van_dev_ultrasound.h>

ackbuff AKB0;//Create a buffer to store messages to be repeatedly sent until an acknowledgement is recieved 

void commandList(Message inData) {
  if (inData.dest == THISNODE) {
    CMD_VAN_NODE(inData);
  }
}

void reportList() {
  AKB0.handleWaiting(); 
}

void setup() {
  Serial.begin(9600);
  AKB0.period = 100;
}

void loop() {
  handleMessage(getMessage(0));
  reportList();
}
