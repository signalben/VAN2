#include "netDef.h"
#include<vanUno.h>
#include<node.h>
#include<van_dev_pid.h>

ackbuff AKB0;//Create a buffer to store messages to be repeatedly sent until an acknowledgement is recieved 
//not used on this node, must be created regardless
van_pid PID0(PID, 2, 4, 3, 5); //create PID device, address PID

//top-level ISRs required for encoder pulse counting, in turn these call PID0 functions
void isrLeft() {
  PID0.isrLeft();
}

void isrRight() {
  PID0.isrRight();
}

//passes messages with a destination device on this node to that device 
void commandList(Message inData) {
  if (inData.dest == THISNODE) {
    CMD_VAN_NODE(inData);
  }
  if (inData.dest == PID) {
    PID0.command(inData);
  }
}

//devices which may send regular messages (timing handled by each device)
void reportList() {
  PID0.autoReport();
}

void setup() {
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(3), isrLeft, CHANGE); //set up top-level ISRs, for encoder pins: 2,3
  attachInterrupt(digitalPinToInterrupt(2), isrRight, CHANGE);
  PID0.period = 20; //update PID at 50hz
  PID0.destination = MOTORS; //PID sends velocity commands to motors
  Message pidInitialState; //send one message to PID setting its initial target velocity (left and right) to stationary (128)
  pidInitialState.set(STD, PC, PID, SET, 128, 128, 1);
  handleMessage(pidInitialState);
}

void loop() {
  //nothing but core network functionality, and autoreporting devices
  handleMessage(getMessage(0));
  reportList();

}
