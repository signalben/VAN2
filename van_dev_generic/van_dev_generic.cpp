//https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/

#include "van_dev_joystick.h"
#include "Arduino.h"
#include <message.h>



van_js::van_js(uint8_t deviceAdress, int a, int b) {

	velPin = a;    // Velocity 36
	hedPin = b;    // Heading  39
	thisDevice = deviceAdress; //JOYSTICK
	destination = 0; //test to pc, should be 0 
	lastReport = 0;
	period = 0;
}

void van_js::command(message inData) {

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
			destination = inData.dat0;
			instantReport();
			destination = suspendedDesination;
		}
		else {
			instantReport();
		}
		return;
	}

}

void van_js::autoReport() {

	if (period == 0) {
		return;
	}

	unsigned long now = millis();
	if ((now - lastReport) < period) {
		return;
	}

	instantReport();
}

void van_js::instantReport() {

	if (destination == 0) {
		return;
	}

	Message buff;
	int velocity = analogRead(velPin);
	int heading = analogRead(hedPin);
	buff.set(thisDevice, destination, REPORT, (velocity >> 4), (heading >> 4), 1);
	handleMessage(buff);
	lastReport = millis();
}

