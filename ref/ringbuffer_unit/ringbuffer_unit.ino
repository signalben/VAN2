uint8_t inbyte;
int activeSerial = 0;
uint16_t mask = B11111111;
esp_now_peer_info_t peerInfo;
uint8_t ringBuff[10][5];
int readFrom = 0;
bool ringBuffAvailable = false;


void ringBufferStore(uint8_t[7] inData) {

  for (int i = 0; i < 10; i++) {

    if (ringBuff[i][0] == 0) {
      memcpy(ringBuff[i][0], inData +1, 5);
      ringBuffOverload = false;
      ringBuffAvailable = true;
      return;
    }
  }
    Serial.println("ESP-NOW ring buffer over capacity");
    return;
}

void ringBufferRead() {

  for (int i = readFrom; i < 10; i++) {

    if (ringBuff[i][0] != 0) {
      memcpy(message[0], ringBuff[i][0], 5);
      for (int j = 0; j < 5; j++) {
        ringBuff[i][j] = 0;
      }

      if (readFrom >= 9) {
        readFrom = 0;
      }
      else {
        readFrom++;
      }
      return;
    }
  }
    ringBuffAvailable = false;
    return;
  }



void ringBufferFlush() {
  memset(ringBuff, 0, sizeof(ringBuff));
  ringBuffAvailable = false;
}

void onReceiveData(uint8_t[7] inData) {

  if (len != 7) {
    Serial.println("ESP-NOW message wrong length");
    return;
  }

  if (data[0] != 0x07) or (data[6] != 0x00) {
    Serial.println("ESP-NOW message start or end invalid");
    return;
  }

  ringBufferStore(inData);
  espMsgAvailable = true;
}



void setup() {
  ringBufferFlush();

}

void loop() {
  // put your main code here, to run repeatedly:

}
