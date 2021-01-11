//Headers are own work:
#include "netDef.h"
#include<vanEsp.h>
#include<node.h>

ackbuff AKB0;//Create a buffer to store messages to be repeatedly sent until an acknowledgement is recieved

void commandList(Message inData) {
  if (inData.dest == THISNODE) {
    CMD_VAN_NODE(inData);
  }
}

void reportList() {
  AKB0.handleWaiting();
}

unsigned long start = 0;
unsigned long now = 0;
int sent = 0;
int correct = 0;
int missed = 0;
int lost = 0;
bool gotResponse;
Message test;
Message returned;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  espNowSetup();
  AKB0.period = 100;
  Serial.println("OK");
  AKB0.period = 100;
  test.setInt(STD, THISNODE, ESP32STATIC, ECHO, 100, 1);
  delay(100);
}

void loop() {
  delay(10);
  handleMessage(test);
  sent++;
  returned = getMessage(3);
  start = millis();
  while (returned.valid == false) {
    returned = getMessage(3);
   // showMessage(returned);
   // Serial.println(returned.valid);
    now = millis();
    if ((now - start) > 30) {
      lost++;
      gotResponse = false;
      break;
    }
    gotResponse = true;
  }
  if (gotResponse) {
    if ((returned.start == STD) & (returned.src == ESP32STATIC) & (returned.dest == THISNODE) & (returned.cmd == RESPONSE) & (returned.dat0 == 0) & (returned.dat1 == 100)) {
      correct++;
    }
    else {
      missed++;
    }
  }
  if (sent >= 1000) {
    Serial.print("sent: ");
    Serial.println(sent);
    Serial.print("correct: ");
    Serial.println(correct);
    Serial.print("corrupted:");
    Serial.println(corrupted);
    Serial.print("missed:");
    Serial.println(missed);
    Serial.print("lost: ");
    Serial.println(lost - corrupted);
    delay(600000);
  }
}
