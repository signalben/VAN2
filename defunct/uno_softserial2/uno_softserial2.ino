#include <SoftwareSerial.h>
 SoftwareSerial mySerial(10, 11); // RX, TX
void setup(){
  mySerial.begin(9600);
  delay(1000);
  mySerial.write("Hello");
}
void loop(){
  mySerial.println("Hello$");
  delay(1000);
}
