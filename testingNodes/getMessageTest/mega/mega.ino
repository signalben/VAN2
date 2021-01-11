//All headers are own work:
#include "netDef.h"
#include<vanMega.h>
#include<node.h>
#include <van_dev_motors.h>

ackbuff AKB0;//Create a buffer to store messages to be repeatedly sent until an acknowledgement is recieved

//For testing PID/motor response, nano_enc sends encoder counts to this node for display via serial0
//Not possible to monitor directly from nano_enc since it only has one serial port, which is needed to close the PID loop
void commandList(Message inData) {
  if (inData.dest == THISNODE) {
    Serial.println(inData.getDataInt());
  }
}

void reportList() {
}

uint8_t sendBytes[] = {STD, 2, 3, 4, 5, 6};
uint8_t checksum = (STD^2)^(3^4)^(5^6);

void doTest(){

  for (int i = 0; i < 6; i++){
  Serial.print(sendBytes[i]);
  Serial.print(",");
  }
  Serial.println(checksum);
  
  Serial1.write(sendBytes, 6);
  Serial1.write(checksum);
  delay(20);
  Serial.println("Received message:");
  showMessage(getMessage(2));
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);

  //send bytes with valid start & checksum
  Serial.print("Test 1, bytes sent:");
  doTest();


  //"corrupt" single byte 2 -> 3
  Serial.print("Test 2, bytes sent:");
  sendBytes[1] = 3;
  doTest();


  //"corrupt" same bit of another byte 4 -> 5
  Serial.print("Test 3, bytes sent:");
  sendBytes[3] = 5;
  doTest();

  //"accidentally" swap two bytes 5 <-> 6
  Serial.print("Test 4, bytes sent:");
  uint8_t holdByte = sendBytes[4];
  sendBytes[4] = sendBytes[5];
  sendBytes[5] = holdByte;
  doTest();

  //send with invalid start (made valid again after test)
  Serial.print("Test 5, bytes sent:");
  sendBytes[0] = 8;
  doTest();
  sendBytes[0] = STD;

  //send with junk byte beforehand
  Serial.print("Test 6, bytes sent:");
  Serial.print(8); //junk
  Serial.print(",");
  
  Serial1.write(8); //junk
  doTest();

  //send with junk byte which happens to be valid start byte
  Serial.print("Test 7, bytes sent:");
  Serial.print(7); //junk
  Serial.print(",");
  
  Serial1.write(7); //junk
  doTest(); 

  //send with junk byte within message
  Serial.print("Test 8, bytes sent:");

  Serial1.write(sendBytes, 6);
  Serial1.write(7); //junk
  Serial1.write(checksum);
  
      for (int i = 0; i < 6; i++){
  Serial.print(sendBytes[i]);
  Serial.print(",");
  }
  
  Serial.print(7); //junk
  Serial.print(",");
  Serial.println(checksum);
  
  delay(20);
  Serial.println("Received message:");
  showMessage(getMessage(2));
}



void loop() {
  delay(6000000);  
}
