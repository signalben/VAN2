#include <message.h>
#include <ringbuff.h>

Message spoofMsg(Message inData) {
  inData.src++;
  inData.dest++;
  inData.cmd++;
  inData.dat0++;
  inData.dat1++;
  inData.valid = true;
  return (inData);
}

Ringbuff RB0;
int count = 0;
Message rData;

void setup() {
  rData.src = 1;
  rData.dest = 1;
  rData.cmd = 1;
  rData.dat0 = 1;
  rData.dat1 = 1;
  rData.valid = false;
  Serial.begin(115200);
  Serial.println("Serial working");
}

void loop() {

  if (count < 40) {
    rData = spoofMsg(rData);
    RB0.Store(rData);
    //Serial.println("Message stored");
  }

  //Serial.println("Status before reading:");
  //RB0.Show();

  if (count > 20) {

    Serial.print("Read message: ");
    Message readData = RB0.Read();
    Serial.print(readData.valid);
    Serial.print(":");
    Serial.print(readData.src);

    //Serial.println("Status after reading:");
    //RB0.Show();
  }

  Serial.print("  Overload: ");
  Serial.print(RB0.Overload);
  Serial.print("  Available: ");
  Serial.print(RB0.Available);
  Serial.print("  msgCount: ");
  Serial.print(RB0.msgCnt);
  Serial.println();

  delay(200);
  count++;
}
