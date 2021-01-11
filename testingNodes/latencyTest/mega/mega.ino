//All headers are own work:
#include "netDef.h"
#include<vanMega.h>
#include<node.h>
#include <van_dev_motors.h>

ackbuff AKB0;//Create a buffer to store messages to be repeatedly sent until an acknowledgement is recieved

void commandList(Message inData) {
  if (inData.dest == THISNODE) {
    CMD_VAN_NODE(inData);
  }
}

void reportList() {
  AKB0.handleWaiting();
}

//variabled used in latency test
unsigned long start = millis();
unsigned long now = 0;
int count = 0;
Message test;
Message returned;

//Single echo message sent
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  Serial.println("OK");
  AKB0.period = 100;
  test.setInt(STD, THISNODE, NANO, ECHO, 100, 1);
  start = micros();
  handleMessage(test);
}

//Prints delay when response received
void loop() {
  handleMessage(getMessage(0)); //Other loop functions left in 
  handleMessage(getMessage(1));
  returned = getMessage(2);
  handleMessage(returned);
  if(returned.dat1 == 100){
    Serial.println(micros());
    Serial.println(start);
    Serial.println(count);
    showMessage(returned);
    delay(10000);
  }
  handleMessage(getMessage(3));
  reportList();
  count++;
}
