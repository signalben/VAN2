#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX
void setup() {
  Serial.begin(9600);
  Serial.println("Testing");
  mySerial.begin(9600);
  mySerial.listen();
}


int  i = 0;
//
//void loop() { // run over and over
//
//
//  if(i != 0){Serial.println();}
//    i = 0;
//
//
// while (mySerial.available() > 0) {
//    message[i] = mySerial.read();
//    i++;
////Serial.print(i);
//
//    for (int j = 0; j < i; j++) {
//      Serial.print(message[j]);
//      message[j] = 0;
//    }
//
//  }
//}


void loop() { // run over and over


  if (mySerial.available() > 0) {
    char a = mySerial.read();
    if(a=='$'){Serial.println("END");}
    else{Serial.print(a);}
  }
}
