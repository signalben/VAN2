//https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/

#include "ultrasound.h"
#include "Arduino.h"

uint8_t object_ID = ultrasound;
uint8_t default_destination = PC; //Default to PC
int trigPin = 11;    // Trigger
int echoPin = 12;    // Echo


int nCall = 0;
int divisor = 10;

void ultrasound_CMD() {
	int recData = (message[3] << 8) | (message[4]); //Recieved data bytes message[3],[4] converted to integer

	if (message[2] == 0x02) {     //Standard set publish rate CMD
		divisor = recData - 1;
	}

	messageValid = false;
}

void ultrasound_PUB() {

	

	if (divisor < 0) { //if negative do not publish
		return;
	}

	if (nCall >= divisor) {

		nCall = 0;
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
		message[0] = object_ID; //Ultrasound ID, 0x10
		message[1] = default_destination; //Default to PC
		message[2] = 0x01;       //Standard reporting data as int CMD
		message[3] = distance >> 8;   //Splits int distance reading over 2 bytes
		message[4] = distance & mask;
		messageValid = true;
		 handleMessage();
		//debugMessage();
		messageValid = false;
	}
	nCall++;
}