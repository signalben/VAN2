//All headers are own work:
#include "netDef.h"
#include<vanMega.h>
#include<node.h>
#include <van_dev_motors.h>

ackbuff AKB0;//Create a buffer to store messages to be repeatedly sent until an acknowledgement is recieved 
van_motors M0(MOTORS,12,3,13,11);

void commandList(Message inData) {

  Serial.print("Message passed to local device: ");
  if (inData.dest == THISNODE) {
    Serial.println("this node (MEGA)");
    //CMD_VAN_NODE(inData);
    
  }
    if (inData.dest == MOTORS) {
      Serial.println("motors");
    //M0.command(inData);
  }
  showMessage(inData);
  Serial.println();
}

void reportList() {
  AKB0.handleWaiting();
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
  Serial3.begin(115200);

  Message dummy;
  dummy.set(STD, MEGA, ESP32BUGGY, REPORT, 12, 34, false);
  
  Serial.println("Test 1, destination: ESP32BUGGY");
  handleMessage(dummy);
  Serial.println();
  
  Serial.println("Test 2, destination: ESP32BUGGY");
  dummy.valid = true;
  handleMessage(dummy);

  Serial.println("Test 3, destination: JOYSTICK");
  dummy.dest = JOYSTICK;
  handleMessage(dummy);

  Serial.println("Test 4, destination: NANO");
  dummy.dest = NANO;
  handleMessage(dummy);

  Serial.println("Test 5, destination: NANO_ENC");
  dummy.dest = NANO_ENC;
  handleMessage(dummy);

  Serial.println("Test 6, destination: PID");
  dummy.dest = PID;
  handleMessage(dummy);

  Serial.println("Test 7, destination: MEGA");
  dummy.dest = MEGA;
  handleMessage(dummy);

  Serial.println("Test 8, destination: MOTORS");
  dummy.dest = MOTORS;
  handleMessage(dummy);

  Serial.println("Test 9, destination: error");
  dummy.dest = 45;
  handleMessage(dummy);
  
}

void loop() {
    
    
delay(6000000);
}
