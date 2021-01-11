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
MPU6050 mpu(Wire); //initialize MPU6050 object (MPU6050_light.h) to get readings from IMU
float AY, ANGY, GZ; //Store IMU readings where IMU0 can access them
unsigned long IRRead; //store IR sensor received values where REM0 can access them

Van_imu IMU0(IMU); //create IMU device, address IMU
Van_roll RL0(ROLLOVER); //create rollover device, addres ROLLOVER
Van_rem REM0(REMOTE); //create remote device, address REMOTE

int IR_RECEIVE_PIN = 15; //specify which pin IR sensor is attached to
IRrecv IrReceiver(IR_RECEIVE_PIN); //initialize reciever object using this pin (IRremote.h)


void REM0Update() {  //function called only by REM0 device
  if (IrReceiver.decode()) {    //decoded IR sensor readings passed to REM0
    IRRead = IrReceiver.results.value;
    REM0.IRReadValue = IRRead;
    IrReceiver.resume(); // Receive the next value
  }
  else {
    REM0.IRReadValue = 0; //0 specifies no reading to REM0
  }
}

void IMU0Update() { //function called only by IMU0 device
  mpu.update();     //update all IMU values for use by IMU0 
  AY = mpu.getAccY();
  GZ = mpu.getGyroZ();
  ANGY = mpu.getAngleY();
}

//passes messages with a destination device on this node to that device 
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

//devices which may send regular messages (timing handled by each device)
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
  Wire.begin(); //initialization for MPU
  mpu.begin();  //initialization for MPU
  mpu.calcOffsets();//initialization for MPU
  IMU0.AYdest = PC;
  IMU0.GZdest = PC;
  IMU0.ANGYdest = ROLLOVER;
  IMU0.period = 100;  //send IMU updates at 10hz
  RL0.destination = MOTORS;
  REM0.period = 100; //send remote updates at 10hz
  REM0.destination = PID;
  AKB0.period = 100; //send waiting for ack response messages at 10hz
}

void loop() {
  //nothing but core network functionality, and autoreporting devices
  handleMessage(getMessage(2));
  handleMessage(getMessage(3));
  reportList();
}
