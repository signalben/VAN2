//https://forum.arduino.cc/index.php?topic=234151.0


#include <SoftwareSerial.h>
SoftwareSerial ss0(10, 11); // RX, TX
void setup() {
  Serial.begin(115200);
  Serial.println("Testing");
  ss0.begin(57600);
  ss0.listen();
}

unsigned long start;
unsigned long now;

void capture_message() {
  uint8_t  n;
  uint8_t  c;
  uint8_t  a;
  uint8_t  b;

  for (int i = 0; i < 5; i++) {

    start = micros();
    while (ss0.available() == 0) {
      now = micros();
      if ((now - start) > 1000) {
        Serial.println("TIME OUT");
        return;
      }
    }

    if (i == 0) {
      n = ss0.read();
      Serial.println(n, HEX);
    }

    else if (i == 1) {
      c = ss0.read();
      Serial.println(c, HEX);
    }

    else if (i == 2) {
      a = ss0.read();
      Serial.println(a, HEX);
    }

    else if (i == 3) {
      b = ss0.read();
      int recvint = (a << 8) | b;
    //  Serial.println(recvint);
      Serial.println(b, HEX);
    }

    else if ((i == 4) and (ss0.read() == '!')) {
      ss0.print("$");
      Serial.println("ACK END");
      return;
    }

    else {
      Serial.println("BAD END");
      return;
    }
  }
  Serial.println("ERROR");
  return;
}

void loop() {

capture_message();
delay(1000);

}
