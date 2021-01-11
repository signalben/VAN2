//Headers are own work:
#include "netDef.h"
#include <vanEsp.h>
#include <node.h>
#include <van_dev_joystick.h>
#include <van_dev_skidsteer.h>

ackbuff AKB0;//Create a buffer to store messages to be repeatedly sent until an acknowledgement is recieved

void commandList(Message inData) {
  if (inData.dest == THISNODE) {
    CMD_VAN_NODE(inData);
  }
}

void reportList() {
}

//variabled used in latency test
unsigned long start = millis();
unsigned long now = 0;
int count = 0;
Message test;
Message returned;

//Single echo message sent
void setup() {
  Serial.begin(115200);
  espNowSetup();
  Serial.println("OK");
  AKB0.period = 100;
  test.setInt(STD, THISNODE, ESP32BUGGY, ECHO, 100, 1);
  start = micros();
  handleMessage(test);
}

void loop() {
  handleMessage(getMessage(0)); //Other loop functions left in 
  returned = getMessage(3);
  handleMessage(returned);
  if (returned.dat1 == 100) {
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
