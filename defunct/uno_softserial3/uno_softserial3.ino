#include <SoftwareSerial.h>
SoftwareSerial ss0(10, 11); // RX, TX

char usID = "u";
int  usDAT = 123;

bool RTS() {
  char r = "0";
  ss0.print("<");
  delay(1000);
  if (ss0.available() > 0) {
    r = ss0.read();
    Serial.print(r);
  }
  else {
    while (ss0.available() == 0) {
      if (ss0.available() > 0) {
        r = ss0.read();
        Serial.print(r);
      }
    }
  }
  if (r == '>') {
    return true;
    Serial.print("RTS OK 1");
  }
  else {
    Serial.print("BAD RESPONSE:");
    Serial.println(r);
    return false;
  }
}

void usPUB() {
  if (RTS() == true) {
    Serial.print("RTS OK 2");
  }
}


void setup() {
  Serial.begin(115200);
  ss0.begin(9600);
  ss0.listen();
  delay(1000);
}

void loop() {
  Serial.print("Sending RTS");
  usPUB();
  Serial.print("Done");
  delay(10000000);
}
