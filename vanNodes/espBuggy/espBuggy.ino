//IRremote: IRreceiveDemo - demonstrates receiving IR codes with IRrecv
//Copyright 2009 Ken Shirriff
//http://arcfn.com

#include "netDef.h"
#include<vanEsp.h>
#include<node.h>
#include<van_dev_imu.h>
#include<van_dev_rollover.h>
#include<van_dev_remote.h>
#include<Wire.h>
#include<MPU6050_light.h>
#include <IRremote.h>

ackbuff AKB0;//Create a buffer to store messages to be repeatedly sent until an acknowledgement is recieved 
MPU6050 mpu(Wire);
Van_imu IMU0(IMU);
Van_roll RL0(ROLLOVER);
Van_rem REM0(REMOTE);

int IR_RECEIVE_PIN = 15;
IRrecv IrReceiver(IR_RECEIVE_PIN);


void REM0Update() {
  if (IrReceiver.decode()) {
    REM0.IRReadValue = IrReceiver.results.value;
    IrReceiver.resume(); // Receive the next value
  }
  else {
    REM0.IRReadValue = 0;
  }
}

void IMU0Update() {
  mpu.update();
  IMU0.AY = mpu.getAccY();
  IMU0.GZ = mpu.getGyroZ();
  IMU0.ANGY = mpu.getAngleY();
}

void commandList(Message inData) {
  if (inData.dest == THISNODE) {
    CMD_VAN_NODE(inData);
  }
  if (inData.dest == ROLLOVER) {
    RL0.command(inData);
  }
  if (inData.dest == REMOTE) {
    REM0.command(inData);
  }
}

void reportList() {
  IMU0.autoReport();
  REM0.autoReport();
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  espNowSetup();
  IrReceiver.enableIRIn();  // Start the receiver
  Wire.begin();
  mpu.begin();
  mpu.calcOffsets();
  IMU0.AYdest = PID;
  IMU0.GZdest = PID;
  IMU0.ANGYdest = ROLLOVER;
  IMU0.period = 0;
  RL0.destination = MOTORS;
  REM0.period = 100;
  REM0.destination = PID;
}

void loop() {
  handleMessage(getMessage(2));
  handleMessage(getMessage(3));
  reportList();
}
