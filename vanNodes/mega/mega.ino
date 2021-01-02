#include "netDef.h"
#include<vanMega.h>
#include<node.h>
#include <van_dev_motors.h>
//https://www.youtube.com/watch?v=Mvgw7XtpaJY demo
ackbuff AKB0;//Create a buffer to store messages to be repeatedly sent until an acknowledgement is recieved 
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
  AKB0.handleWaiting();
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
  Serial3.begin(115200);
  AKB0.period = 100;
  M0.period = 0;
  M0.destination = PC;
  
}

void loop() {
    handleMessage(getMessage(0));
    handleMessage(getMessage(1));
    handleMessage(getMessage(2));
    handleMessage(getMessage(3));
  reportList();
}
