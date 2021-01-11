#include "netDef.h"
#include<vanMega.h>
#include<node.h>
#include <van_dev_motors.h>

ackbuff AKB0;//Create a buffer to store messages to be repeatedly sent until an acknowledgement is recieved 

void commandList(Message inData) {
}

void reportList() {
  
}

void setup() {
  Serial.begin(115200);
  Serial.println("Test 1: "); 
  AKB0.period = 1;
  AKB0.show(); //show empty buffer
  AKB0.handleWaiting();

  Serial.println("Test 2: "); 
  message dummy;
  dummy.set(STD, REMOTE, PID, SET, 255, 255, true);
  AKB0.add(dummy);
  AKB0.show();//show message in buffer
  AKB0.handleWaiting();
  
  Serial.println("Test 3: "); 
  Serial.println(AKB0.contains(dummy)); //1 yes
  AKB0.add(dummy);
  AKB0.show();//show message in buffer
  AKB0.handleWaiting();
  
  Serial.println("Test 4: "); 
  dummy.set(STD, IMU, PID, SET, 255, 254, true);
  Serial.println(AKB0.contains(dummy)); //0 no
  
  Serial.println("Test 5: "); 
  AKB0.add(dummy);
  AKB0.show();//no change
  AKB0.handleWaiting();

  Serial.println("Test 6: "); 
  dummy.set(STD, REMOTE, PID, SET, 255, 254, true);
  AKB0.clear(dummy);//no change not quite right
  AKB0.show();//show message in buffer
  AKB0.handleWaiting();

  Serial.println("Test 7: ");
  dummy.set(STD, REMOTE, PID, SET, 255, 255, true);
  AKB0.clear(dummy);//cleared, exactly right
  AKB0.show();//show message in buffer
  AKB0.handleWaiting();
  

  Serial.println("Test 8: ");
  AKB0.add(dummy);
  AKB0.show();//show message in buffer
  dummy.set(STD, REMOTE, PID, SET, 47, 52, true);
  AKB0.cancel(dummy);
  AKB0.show();//show message in buffer
  AKB0.handleWaiting();

  Serial.println("Test 9: ");
}



void loop() {
  AKB0.period = 0;
AKB0.handleWaiting();
}
