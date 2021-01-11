#include "netDef.h"
#include<vanUno.h>
#include<node.h>
#include<van_dev_ultrasound.h>

ackbuff AKB0;//Create a buffer to store messages to be repeatedly sent until an acknowledgement is recieved 
van_us US0(ULTRASOUND, 11, 12); //create ultraousnd device, address ULTRASOUND, trig & echo pins

//passes messages with a destination device on this node to that device 
void commandList(Message inData) {
  if (inData.dest == THISNODE) {
    CMD_VAN_NODE(inData);
  }
  if (inData.dest == ULTRASOUND) {
    US0.command(inData);
  }
}

//devices which may send regular messages (timing handled by each device)
void reportList() {
  AKB0.handleWaiting();
  US0.autoReport(); 
}

void setup() {
  Serial.begin(115200);
  US0.period = 100; //take ultrasound reads at 10hz
  US0.destination = PC; //send these readings to the PC 
  AKB0.period = 100; //send waiting for ack response messages at 10hz
}

void loop() {
  //nothing but core network functionality, and autoreporting devices
  handleMessage(getMessage(0));
  reportList();
}
