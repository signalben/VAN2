#include "netDef.h"
#include<vanUno.h>
#include<node.h>
#include<van_dev_pid.h>
van_pid PID0(PID, 2, 4, 3, 5);

void isrLeft() {
  PID0.isrLeft();
}

void isrRight() {
  PID0.isrRight();
}

void commandList(Message inData) {
  if (inData.dest == THISNODE) {
    CMD_VAN_NODE(inData);
  }
  if (inData.dest == PID) {
    PID0.command(inData);
   // Serial.print("target");
  }
}

void reportList() {
  PID0.autoReport();
}

void setup() {
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(3), isrLeft, CHANGE);
  attachInterrupt(digitalPinToInterrupt(2), isrRight, CHANGE);
  PID0.period = 50;
  PID0.destination = MOTORS;
  //PID0.lTarget = 5;
  //Serial.print("alive");
    Message temp;
  temp.set(PC, PID, SET, 128, 128, 1);
  handleMessage(temp);
}

void loop() {
  handleMessage(getMessage(0));
  reportList();

}
