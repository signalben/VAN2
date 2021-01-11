struct message {
  uint8_t src;
  uint8_t dest;
  uint8_t cmd;
  uint8_t dat0;
  uint8_t dat1;
  bool valid;
};


int count = 0;

typedef struct message Message;
Message rData;

Message ringBuff[10];
int readFrom = 0;
bool ringBuffAvailable = false;
bool ringBuffOverload = false;

Message doesStuffToMsg(Message inData) {
  inData.src++;
  inData.dest++;
  inData.cmd++;
  inData.dat0++;
  inData.dat1++;
  inData.valid = true;
  return (inData);
}

void ringBufferStore(Message inData) {

  for (int i = 0; i < 10; i++) {  //For every message slot in ringbuffer

    if (ringBuff[i].valid == false) { //Check not already filled with valid message
      ringBuffOverload = false;
      ringBuff[i] = inData;
      ringBuffAvailable = true;
      return;
    }
  }
  Serial.println("ESP-NOW ring buffer over capacity");
  return;
}

Message ringBufferRead() {

  for (int i = readFrom; i < 10; i++) {
    if (ringBuff[i].valid) { //if valid message in ringbuff slot
      Message outData = ringBuff[i]; //Retreive message
      ringBuff[i].valid = false; //then empty that slot
      readFrom = i + 1; //Set starting slot for next run
      return outData;
    }
  }

  readFrom = 0;
  Serial.println("Note: reading from slot zero again");

  for (int i = readFrom; i < 10; i++) {
    if (ringBuff[i].valid) { //if valid message in ringbuff slot
      Message outData = ringBuff[i]; //Retreive message
      ringBuff[i].valid = false; //then empty that slot
      readFrom = i + 1; //Set starting slot for next run
      return outData;
    }
  }

  ringBuffAvailable = false;
  Serial.println("Error- Nothing in ringBuffer!!!!!!!!!");
  return ringBuff[0]; //in this case no valid messages, and ringBuff[0].valid = false
}


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
  rData = doesStuffToMsg(rData);
  ringBufferStore(rData);
  Serial.println("Another message stored");
  }

  Serial.println("Status before reading:");
  for (int i = 0; i < 10; i++) {
    Serial.print(ringBuff[i].valid);
    Serial.print(":");
    Serial.print(ringBuff[i].src);
    Serial.print("_");
  }
  Serial.println();

  if (count > 20) {

    Serial.println("Read message:");
    Message readData = ringBufferRead();
    Serial.print(readData.valid);
    Serial.print(":");
    Serial.print(readData.src);
    Serial.println();

    Serial.println("Status after reading:");
    for (int i = 0; i < 10; i++) {
      Serial.print(ringBuff[i].valid);
      Serial.print(":");
      Serial.print(ringBuff[i].src);
      Serial.print("_");
    }
    Serial.println();
  }

  delay(1000);
  count++;
}
