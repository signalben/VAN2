//IRremote: IRreceiveDemo - demonstrates receiving IR codes with IRrecv
//Copyright 2009 Ken Shirriff
//http://arcfn.com


#include <IRremote.h>
int IR_RECEIVE_PIN = 15;
IRrecv IrReceiver(IR_RECEIVE_PIN);

void setup() {
    Serial.begin(115200);
    IrReceiver.enableIRIn();  // Start the receiver
}

void loop() {
    if (IrReceiver.decode()) {
        Serial.println(IrReceiver.results.value, HEX);
        IrReceiver.resume(); // Receive the next value
    }
    delay(10000);
}
