//https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/

#include "van_dev_ultrasound.h"
#include "Arduino.h"
#include <message.h>

van_us::van_us(uint8_t deviceAdress, int a, int b) {

	trigPin = a;    // Trigger 11
	echoPin = b;    // Echo 12
	thisDevice = deviceAdress; //ULTRASOUND
	destination = 0; //test to pc, should be 0 
	lastReport = 0;
	period = 0;
	obstructed = 1500;
}

void van_us::command(message inData) {

	if (inData.cmd == PERIOD) {
		period = inData.getDataInt();
		return;
	}

	if (inData.cmd == SETDEST) {
		destination = inData.dat1;
		return;
	}

	if (inData.cmd == REPORTNOW) {
		if (inData.dat1 != 0) {
			uint8_t suspendedDesination = destination;
			destination = inData.dat1;
			instantReport();
			destination = suspendedDesination;
		}
		else {
			instantReport();
		}
		return;
	}

	if (inData.cmd == PARAM0) {
		obstructed = inData.getDataInt();
	}

}

void van_us::autoReport() {

	if (period == 0) {
		return;
	}

	unsigned long now = millis();
	if ((now - lastReport) < period) {
		return;
	}

	instantReport();
}

void van_us::instantReport() {

	if (destination == 0) {
		return;
	}

	pinMode(trigPin, OUTPUT);
	digitalWrite(trigPin, LOW);
	delayMicroseconds(5);
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin, LOW);
	pinMode(echoPin, INPUT);
	int distance = int(pulseIn(echoPin, HIGH));

	Message buff;
	buff.setInt(STD, thisDevice, destination, REPORT, distance, 1);
	handleMessage(buff);

	assessDanger(distance);

	lastReport = millis();
}

void van_us::assessDanger(int distance) {
	bool clearAhead;
	static bool oldClearAhead = false;

	if (distance < obstructed) {
		clearAhead = false;
	}
	else {
		clearAhead = true;
	}

	if (clearAhead != oldClearAhead) {//Has changed state
		Message toMotors;

		if (clearAhead) {//obsticle cleared
			toMotors.set(STD, thisDevice, MOTORS, PARAM0, 255, 255, 1); //clear ahead
		}
		else {//obsticle detected
			toMotors.set(STD, thisDevice, MOTORS, PARAM0, 0, 0, 1); //obsticle detected
		}
		AKB0.cancel(toMotors);//cancel previous ACK waiting
		AKB0.add(toMotors); //add message to acknowledge message send list
	}
	oldClearAhead = clearAhead;
}

