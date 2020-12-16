#include "van_dev_remote.h"
#include "Arduino.h"
#include <message.h>

van_rem::van_rem(uint8_t deviceAdress) {
	thisDevice = deviceAdress; //IMU
	IRReadValue = 0;
	lastReport = 0;
	period = 0;
}

void van_rem::command(message inData) {

	if (inData.cmd == PERIOD) {
		period = inData.getDataInt();
		return;
	}

	if (inData.cmd == SETDEST) {
		destination = inData.dat1;
		return;
	}

	if (inData.cmd == REPORTNOW) {
		instantReport();
		return;
	}

}

void van_rem::autoReport() {

	if (period == 0) {
		return;
	}

	unsigned long now = millis();
	if ((now - lastReport) < period) {
		return;
	}

	instantReport();
}

void van_rem::instantReport() {

	const unsigned long left = 16716015, right = 16734885, up = 16718055, down = 16730805, repeat = 4294967295;
	static unsigned long previousIRRead;
	static bool stopped;
	int lVel, rVel;
	int scaleVel = 127;
	
	REM0Update();
	Message buff;

	if (IRReadValue) {
		stopped = false;
		if (IRReadValue == repeat) {
			IRReadValue = previousIRRead;
		}
		switch (IRReadValue) {
		case left:
			lVel = -1;
			rVel = 1;
			break;
		case right:
			lVel = 1;
			rVel = -1;
			break;
		case up:
			lVel = 1;
			rVel = 1;
			break;
		case down:
			lVel = -1;
			rVel = -1;
			break;
		default:
			lVel = 0;
			rVel = 0;
			break;
		}

		previousIRRead = IRReadValue;
		lVel = (lVel * scaleVel) + 128;
		rVel = (rVel * scaleVel) + 128;
		lVel = constrain(lVel, 0, 255);
		rVel = constrain(rVel, 0, 255);

		buff.set(STD, thisDevice, destination, SET, uint8_t(lVel), uint8_t(rVel), 1);
		handleMessage(buff);
		//showMessage(buff);
	}
	else {
		if (stopped == false) {
			stopped = true;
			buff.set(STD, thisDevice, destination, SET, 128, 128, 1);
			handleMessage(buff);
			//showMessage(buff);
		}
	}


	lastReport = millis();
}
