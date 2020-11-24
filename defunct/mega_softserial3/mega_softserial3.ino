#include <SoftwareSerial.h>
SoftwareSerial ss0(10, 11); // RX, TX
void setup() {
  Serial.begin(115200);
  Serial.println("Testing");
  ss0.begin(9600);
  ss0.listen();
}



char buff[99];

void capture_message() {
  Serial.print("In buffer:");
  Serial.println(ss0.available());
  int  i = 0;
  while (ss0.available() > 0) {
//    Serial.print(ss0.read());
      char a = char(ss0.read());
    //   buff[i] = a;
      Serial.println(a);
    //  Serial.print(i);
    //  Serial.println(buff[i]);
    i++;
  }
  Serial.println("ACK sent");
  ss0.print("$");
}

void print_buffer() {
  int j = 0;
  for ( j = 0; j < 99; j++) {
    char a = buff[j];
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
      print_buffer();
    }
  }
}
