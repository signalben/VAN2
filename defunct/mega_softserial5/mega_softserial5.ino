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

int buff[99];

void capture_message() {
int  i = 0;
uint8_t  a;
uint8_t  b;
char inbyte;

  while (true) {
    
    start = micros();

    while (ss0.available() == 0) {
      now = micros();
      if ((now - start) > 1000) {
        Serial.println("TIME OUT");
        return;
      }
    }

    if (ss0.available() > 0) {
      inbyte = char(ss0.read());
      if (inbyte == '!') {
        ss0.print("$");
        Serial.println("ACK END");
        return;
      }
      else {
       // Serial.print(inbyte);
        if (i == 1) {
        a = inbyte;
        }
        if (i == 2) {
         b = inbyte;
          int recvint = (a << 8) | b;
          Serial.println(recvint);
        }
      }
      i++;
    }
  }
}

void print_buffer() {
  int j = 0;
  for ( j = 0; j < 99; j++) {
    //    int a = buff[j];
    Serial.println(buff[j]);
  }
}

void loop() {

  if (ss0.available() > 0) {
    char a = ss0.read();
    if (a == '<') {
      Serial.println("RTS recieved");
      ss0.print(">");
      capture_message();
      //      print_buffer();
    }
  }
}
