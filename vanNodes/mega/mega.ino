#include "netDef.h"
#include<vanMega.h>
#include<node.h>
#include <van_dev_motors.h>

ackbuff AKB0;//Create a buffer to store messages to be repeatedly sent until an acknowledgement is recieved 
van_motors M0(MOTORS,12,3,13,11); //create motors device, address MOTORS, pins for duty, direction on two channels

//passes messages with a destination device on this node to that device 
void commandList(Message inData) {
  if (inData.dest == THISNODE) {
    CMD_VAN_NODE(inData);
  }
    if (inData.dest == MOTORS) {
    M0.command(inData);
  }
}

//devices which may send regular messages (timing handled by each device)
void reportList() {
  AKB0.handleWaiting();
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
  Serial3.begin(115200);
  AKB0.period = 100; //send waiting for ack response messages at 10hz
  M0.period = 0; //motors does not need to send messages regularly
  M0.destination = PC; //when change of safety condition, motors does notify PC 
  
}

void loop() {
  //nothing but core network functionality, and autoreporting devices
    handleMessage(getMessage(0));
    handleMessage(getMessage(1));
    handleMessage(getMessage(2));
    handleMessage(getMessage(3));
  reportList();
}
