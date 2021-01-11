//Headers are own work:
#include "netDef.h"
#include <vanEsp.h>
#include <node.h>


ackbuff AKB0;//Create a buffer to store messages to be repeatedly sent until an acknowledgement is recieved 


void commandList(Message inData) {
  if (inData.dest == THISNODE) {
    CMD_VAN_NODE(inData);
  }

}

void reportList() {

}

void setup() {
  Serial.begin(115200);
  espNowSetup();
}

void loop() {
  //handleMessage(getMessage(0));
  handleMessage(getMessage(3));
  reportList();
}
