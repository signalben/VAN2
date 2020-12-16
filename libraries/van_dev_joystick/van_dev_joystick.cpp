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

/*	if (inData.cmd == PERIOD) {
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
	*/
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

	int velocity = (analogRead(velPin)/16);	//range 0, 4095 => 0, 255
	int heading =  (analogRead(hedPin)/16);
	static int inUse = 0;

	if ((105 < velocity) && (velocity < 135)) {
		velocity = 128;
	}
	else {
		inUse++;
	}

	if ((105 < heading) && (heading < 135)) {
		heading = 128;
	}
	else {
		inUse++;
	}

	 if (inUse >= 3) {
		inUse = 3;
		Message buff;
		buff.set(STD, thisDevice, destination, SET, uint8_t(velocity), uint8_t(heading), 1);
		handleMessage(buff);
	}
	 if ((velocity == 128) && (heading == 128)) { //don't keep sending stationary messages- other device may be in control
		 inUse = 0;
	 }

	lastReport = millis();
}

