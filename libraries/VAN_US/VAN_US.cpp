//https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/

#include "VAN_US.h"
#include "Arduino.h"
#include <message.h>

uint8_t thisDevice = ultrasound;
uint8_t default_destination = PC; //Default to PC
int trigPin = 11;    // Trigger
int echoPin = 12;    // Echo

unsigned long lastReport = 0;
unsigned int period = 1000;

void CMD_VAN_US(message inData) {

	if (inData.cmd == PERIOD) {
		period = inData.getInt();
	}

}

void REP_AUTO_VAN_US() {

	unsigned long now = millis();
	if ((now - lastReport) < period) {
		return;
	}

	REP_NOW_VAN_US();
}

void REP_NOW_VAN_US() {

	pinMode(trigPin, OUTPUT);
	digitalWrite(trigPin, LOW);
	delayMicroseconds(5);
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin, LOW);
	pinMode(echoPin, INPUT);
	int distance = int(pulseIn(echoPin, HIGH));

	Message buff;
	buff.setInt(thisDevice, default_destination, REPORT, distance, 1);
	handleMessage(buff);
}