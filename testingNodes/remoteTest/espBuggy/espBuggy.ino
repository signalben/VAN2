//Headers are own work:
#include "netDef.h"
#include<vanEsp.h>
#include<node.h>
#include<van_dev_imu.h>
#include<van_dev_rollover.h>
#include<van_dev_remote.h>

//Headers are not own work:
#include<Wire.h>
#include<MPU6050_light.h>
#include <IRremote.h>

ackbuff AKB0;//Create a buffer to store messages to be repeatedly sent until an acknowledgement is recieved

unsigned long IRRead;

Van_rem REM0(REMOTE);

int IR_RECEIVE_PIN = 15;
IRrecv IrReceiver(IR_RECEIVE_PIN);
unsigned long start = 0;
unsigned long now = 0;
int correct = 0;
int corrupt = 0;
int lost = 0;
int trial = 1;
bool gotResponse = false;

void REM0Update() {

  if (IrReceiver.decode()) {
    IRRead = IrReceiver.results.value;
    if (IRRead == 4294967295) {
      correct++;
    }
    else {
      corrupt++;
    }
    IrReceiver.resume(); // Receive the next value
  }
  now = millis();
  if ((now - start) >= 5000) {
    int expected = 47;
    lost = expected - (correct + corrupt);
    Serial.print("trial: ");
    Serial.println(trial);
    Serial.print("correct: ");
    Serial.println(correct);
    Serial.print("corrupt: ");
    Serial.println(corrupt);
    Serial.print("lost: ");
    Serial.println(lost);
    Serial.println();
    delay(3000);
    IrReceiver.resume(); // Receive the next value

    Serial.println("Ready");
    Serial.println();
    while (!IrReceiver.decode()) {
      delay(1);
    }

    Serial.println("Started");
    Serial.println();
    IrReceiver.resume(); // Receive the next value
    start = millis();
    correct = 0;
    corrupt = 0;
    lost = 0;
    trial++;
  }
}

void commandList(Message inData) {

}

void reportList() {

}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  espNowSetup();
  IrReceiver.enableIRIn();  // Start the receiver
  Serial.println("Ready");
  Serial.println();
  while (!IrReceiver.decode()) {
    //Serial.println("wait");
    delay(1);
    start = millis();
  }
  Serial.println("Started");
  Serial.println();
  IrReceiver.resume(); // Receive the next value
}

void loop() {
  REM0Update();
}
