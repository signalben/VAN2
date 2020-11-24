
#include "netDef.h"
#include<vanUno.h>
#include<node.h>
#include<VAN_US.h>



void commandList(Message inData) {
  if (inData.dest == thisNode) {
    CMD_VAN_NODE(inData);
  }
}

void reportList() {
  REP_AUTO_VAN_US();
}

Message dummy;

void setup() {
  Serial.begin(115200);
  verboseLog = true;
  errorLog = true;
  Vlog("Hello");
  Elog("world");
}

void loop() {
  handleMessage(getMessage(0));
  dummy.set(ultrasound, thisNode, PERIOD, 0, 0, 1);
  dummy.setInt(100);
  //handleMessage(dummy);
  delay(10);         
  reportList();
}
