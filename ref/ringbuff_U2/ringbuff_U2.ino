struct message {
  uint8_t src;
  uint8_t dest;
  uint8_t cmd;
  uint8_t dat0;
  uint8_t dat1;
  bool valid;
};
typedef struct message Message;

class ringbuff {
  private:
    Message storage[10]; //Contains the buffered messages
    int readFrom = 0;    //Keeps track of oldest message location (buffer starting point)
    
  public:
    bool Available = false;
    bool Overload = false;
    int msgCnt = 0;

    void Store(Message inData) {
      Available = true;
      for (int i = 0; i < 10; i++) { //For every message slot in storage
        if (storage[i].valid == false) { //Check not already filled with valid message
          Overload = false;
          storage[i] = inData;
          msgCnt++;
          return;
        }
      }
      //Serial.println("ringBuffer over capacity");
      Overload = true;
      //return;
    }

    Message Read() {
      for (int i = readFrom; i < 10; i++) {
        if (storage[i].valid) { //if valid message in storage slot
          Message outData = storage[i]; //Retreive message
          storage[i].valid = false; //then empty that slot
          msgCnt--;
          readFrom = i + 1; //Set advance slot for next call
          return outData;
        }
      }
      readFrom = 0;
      //Serial.println("Note: reading from slot zero again");
      for (int i = readFrom; i < 10; i++) {
        if (storage[i].valid) { //if valid message in storage slot
          Message outData = storage[i]; //Retreive message
          storage[i].valid = false; //then empty that slot
          msgCnt--;
          readFrom = i + 1; //Set advance slot for next call
          return outData;
        }
      }
      Available = false;
      Overload = false;
      //Serial.println("Nothing in buffer!");
      return storage[0]; //in this case no valid messages, and storage[0].valid = false
    }

    void Show() {
      for (int i = 0; i < 10; i++) {
        Serial.print(storage[i].valid);
        Serial.print(":");
        Serial.print(storage[i].src);
        Serial.print("_");
      }
      Serial.println();
    }
};
typedef class ringbuff Ringbuff;

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
