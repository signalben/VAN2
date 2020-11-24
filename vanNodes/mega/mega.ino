#include "netDef.h"
#include<vanEsp.h>

void echoMessage(Message inData){
  Message buff = inData;
  buff.src = inData.dest;
  buff.dest = inData.src;
  buff.cmd = 4; // 4= echo response
  handleMessage(buff);
}

void commandList(Message inData) {
  if(inData.dest == thisNode){
    //assuming CMD = 3 = echo
    echoMessage(inData);
  }
  //showMessage(inData);
}

Message dummy;

void setup() {
  Serial.begin(115200);
  verboseLog = true;
  errorLog = true;
  Vlog("Hello");
  Elog("world");
  espNowSetup();
}

void loop() {
  handleMessage(getMessage(0));
  dummy.set(dummyobject, thisNode, 3, 123, 45, 1);
  handleMessage(dummy);
  delay(1000);                       // wait for a second
}
