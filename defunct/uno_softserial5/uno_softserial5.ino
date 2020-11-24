#include <SoftwareSerial.h>
SoftwareSerial ss0(10, 11); // RX, TX

char usID = 'u';
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

bool RTS() {
  char r = "0";
  ss0.print("<");
  start = micros();

  while (ss0.available() == 0) {
    now = micros();
    if ((now - start) > 1000) {
      break;
    }
  }

  if (ss0.available() > 0) {
    r = ss0.read();
  }
  else {
    Serial.println("No timely response");
    return false;
  }

  if (r == '>') {
    return true;
  }
  else {
    Serial.print("Bad response to RTS");
    return false;
  }
}


void usPUB() {
  if (RTS() == true) {
    Serial.println("RTS GOOD");
    ss0.print(usID);
    uint8_t a = usDAT >> 8;
    uint8_t b = usDAT & mask;
    ss0.write(a);
    ss0.write(b);
    ss0.print(END);
    if (ACK() == true) {
      Serial.println("ACK GOOD");
    }

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
  delay(500);
  usDAT++;
}
