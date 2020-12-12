#include "netDef.h"
#include<vanEsp.h>
#include<node.h>

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
  handleMessage(getMessage(0));
  reportList();
}
