struct message {
  uint8_t src;
  uint8_t dest;
  uint8_t cmd;
  uint8_t dat0;
  uint8_t dat1;
  bool valid;
};

typedef struct message Message;
Message rData;

Message doesStuffToMsg(Message inData) {
  inData.src++;
  inData.dest++;
  inData.cmd++;
  inData.dat0++;
  inData.dat1++;
  inData.valid = true;
  return (inData);
}

void setup() {
  rData.src = 1;
  rData.dest = 2;
  rData.cmd = 3;
  rData.dat0 = 6;
  rData.dat1 = 12;
  rData.valid = false;
  Serial.begin(115200);
  Serial.println("Serial working");

}

void loop() {

  Serial.println("rData: ");
  Serial.println(rData.src);
  Serial.println(rData.dest);
  Serial.println(rData.cmd);
  Serial.println(rData.dat0);
  Serial.println(rData.dat1);
  Serial.println(rData.valid);
  Serial.println();
  delay(5000);

  rData = doesStuffToMsg(rData);
}

//Unit test passed, type Message behaves as expected
