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
MPU6050 mpu(Wire);
float AY, ANGY, GZ;
unsigned long IRRead;

Van_imu IMU0(IMU);
Van_roll RL0(ROLLOVER);
Van_rem REM0(REMOTE);

int IR_RECEIVE_PIN = 15;
IRrecv IrReceiver(IR_RECEIVE_PIN);


void REM0Update() {
  if (IrReceiver.decode()) {
    IRRead = IrReceiver.results.value;
    REM0.IRReadValue = IRRead;
    IrReceiver.resume(); // Receive the next value
  }
  else {
    REM0.IRReadValue = 0;
  }
}

void IMU0Update() {
  mpu.update();
  AY = mpu.getAccY();
  GZ = mpu.getGyroZ();
  ANGY = mpu.getAngleY();
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
    if (inData.dest == IMU) {
    IMU0.command(inData);
  }
}

void reportList() {
  IMU0.autoReport();
  REM0.autoReport();
  AKB0.handleWaiting();
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  espNowSetup();
  IrReceiver.enableIRIn();  // Start the receiver
  Wire.begin();
  mpu.begin();
  mpu.calcOffsets();
  IMU0.AYdest = PC;
  IMU0.GZdest = PC;
  IMU0.ANGYdest = ROLLOVER;
  IMU0.period = 100;
  RL0.destination = MOTORS;
  REM0.period = 100;
  REM0.destination = PID;
  AKB0.period = 100;
}

void loop() {
  handleMessage(getMessage(2));
  handleMessage(getMessage(3));
  reportList();
}
