#define ultrasound_h
#include "Arduino.h"

pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
int trigPin = 11;    // Trigger
int echoPin = 12;    // Echo

object ultrasound;  // Create an object
ultrasound.ID = 0x10;
ultrasound.destination = 0x03;
ultrasound.nCall = 0;
ultrasound.divisor = 100;

void ultrasound.command() {
  int recData = (message[3] << 8) | (message[4]); //Recieved data bytes message[3],[4] converted to integer

  if (message[2] = 0x02) {     //Standard set publish rate CMD
    ultrasound.divisor = recData - 1;
  }

  messageValid = false;
}

void ultrasound.publish() {

  if (u_divisor < 0) { //if negative do not publish
    return;
  }

  if (ultrasound.nCall >= u_divisor) {

    ultrasound.nCall = 0;
    // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
    // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Read the signal from the sensor: a HIGH pulse whose
    // duration is the time (in microseconds) from the sending
    // of the ping to the reception of its echo off of an object.
    pinMode(echoPin, INPUT);
    //    duration = pulseIn(echoPin, HIGH);
    //    if (duration > 32767) {
    //      duration = 32767;
    //    }
    int distance = int(pulseIn(echoPin, HIGH));
    //int distance = int(duration);
    //Serial.println(duration);
    message[0] = 0x10; //Ultrasound ID
    message[1] = destination; //Default to PC
    message[2] = 0x01;       //Standard reporting data as int CMD
    message[3] = distance >> 8;   //Splits int distance reading over 2 bytes
    message[4] = distance & mask;
    messageValid = true;
    //  handleMessage();
    debugMessage();
    messageValid = false;
  }
  ultrasound.nCall++;
}
