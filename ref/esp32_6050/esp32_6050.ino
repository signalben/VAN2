//https://www.instructables.com/Connecting-MPU6050-With-ESP32/
//REF MPU_6050 example PDF
#include <Wire.h>
#include <MPU6050_light.h>

MPU6050 mpu(Wire);
void setup() {
  Serial.begin(500000);
  Wire.begin();
  mpu.begin();
  mpu.calcOffsets();
}

float AX, AY, AZ, brightness, Temp, GX, GY, GZ, AAX, AAY, ANGX, ANGY, ANGZ;
unsigned long previousTimestamp;

void loop() {

  mpu.update();
  //Read Acceleration Values
  AX = mpu.getAccX();
  AY = mpu.getAccY();
  AZ = mpu.getAccZ();

  //Read Gyroscope Values
  GX = mpu.getGyroX();
  GY = mpu.getGyroY();
  GZ = mpu.getGyroZ();

  //Read Acceleration Angle Values
  AAX = mpu.getAccAngleX();
  AAY = mpu.getAccAngleY();

  //Read Angle Values
  ANGX = mpu.getAngleX();
  ANGY = mpu.getAngleY();
  ANGZ = mpu.getAngleZ();

  //Read Temperature Values
  Temp = mpu.getTemp();

  SerialprintData();
  //SerialprintDataGraph();
}

void SerialprintData() {

  unsigned long timestamp = millis();

  while (timestamp - previousTimestamp < 2) {
    timestamp = millis();
  }

  Serial.print(timestamp / 1000);
  Serial.print(".");
  int remainder = timestamp % 1000;
  if (remainder < 10) {
    Serial.print("0");
  }
  if (remainder < 100) {
    Serial.print("0");
  }
  Serial.print(remainder);

  previousTimestamp = timestamp;

 /* Serial.print(",");
  Serial.print(AX);
  Serial.print(",");
  Serial.print(AY);
  Serial.print(",");
  Serial.print(AZ);
  Serial.print(",");

  Serial.print(GX);
  Serial.print(",");
  Serial.print(GY);
  Serial.print(",");
  Serial.print(GZ);
  Serial.print(",");*/

  Serial.print(ANGX);
  Serial.print(",");
  Serial.print(ANGY);
  Serial.print(",");
  Serial.print(ANGZ);
  Serial.print(",");

  Serial.println(Temp);
}

void SerialprintDataGraph() {
    unsigned long timestamp = millis();

  while (timestamp - previousTimestamp < 5) {
    timestamp = millis();
  }
  Serial.println(ANGX);
  Serial.print(",");
  Serial.print(ANGY);
  Serial.print(",");
  Serial.println(ANGZ);
  previousTimestamp = timestamp;
}
