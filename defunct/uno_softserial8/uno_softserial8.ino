#include <SoftwareSerial.h>
SoftwareSerial ss0(10, 11); // RX, TX

uint8_t usID = 0x11;
uint8_t usCMD = 0x22;
uint8_t usID2 = 0x33;
uint8_t usCMD2 = 0x44;
char END = '!';

int usDAT = 1;
uint16_t mask   = B11111111;
unsigned long start;
unsigned long now;

bool ACK() {
  char r = "0";
  start = micros();

  while (ss0.available() == 0) {
    now = micros();
    if ((now - start) > 1000) {
      break;
    }
  }

  //Serial.println(ss0.available());

  if (ss0.available() > 0) {
    r = ss0.read();
  }
  else {
    Serial.println("NO ACK");
    return false;
  }

  if (r == '$') {
    return true;
  }
  else {
    Serial.print("Bad ACK");
    return false;
  }
}

void usPUB() {

    ss0.write(usID);
    ss0.write(usCMD);
    ss0.write(usID2);
    ss0.write(usCMD2);
    uint8_t a = usDAT >> 8;
    uint8_t b = usDAT & mask;
    ss0.write(a);
    ss0.write(b);
    ss0.print(END);
    if (ACK() == true) {
      Serial.println("ACK GOOD");
  }
}


void setup() {
  Serial.begin(115200);
  ss0.begin(57600);
  ss0.listen();
  //delay(1000);
}

void loop() {
  //Serial.println("Sending RTS");
  usPUB();
  //Serial.println("Done");
  delay(100);
  usDAT++;
}
