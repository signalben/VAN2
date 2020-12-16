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
	obstructed = 750;
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

	static bool stopped = false;

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

	/*Message buff;
	buff.setInt(STD, thisDevice, destination, REPORT, distance, 1);
	handleMessage(buff);*/

	Message toMotors;
	if (distance > obstructed) {
		toMotors.set(STD, thisDevice, IMU, PARAM0, 255, 255, 1); //OK to proceed
		stopped = false;
		AKB0.cancel(toMotors);		//cancels repeating of motor disable message (regardless of response, since conditions have changed)
		handleMessage(toMotors);
	}
	else if (stopped == false) {
		toMotors.set(STD, thisDevice, IMU, PARAM0, 0, 0, 1); //Avoid crash
		AKB0.add(toMotors); //add message to accknowledge message send list
	//Message brake;
		//brake.set(STD, thisDevice, PID, SET, 128, 128, 1);
		//handleMessage(brake);
		stopped = true;
	}

	lastReport = millis();
}

