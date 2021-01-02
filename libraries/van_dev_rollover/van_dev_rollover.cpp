
#include "van_dev_rollover.h"
#include "Arduino.h"
#include <message.h>
#include "ackbuff.h"

van_roll::van_roll(uint8_t deviceAdress) {

	thisDevice = deviceAdress; //ROLLOVER
	destination = 0; //test to pc, should be 0 
	angle = 0;
}

void van_roll::command(message inData) {

	if (inData.cmd == PARAM4) {
		angle = inData.getDataInt();
		assessDanger(angle);
		return;
	}

	if (inData.cmd == SETDEST) {
		destination = inData.dat1;
		return;
	}

}

void van_roll::assessDanger(int angle) {
	bool upright;
	static bool oldUpright = false;

	if ((angle < -45) || (angle > 45)) {
		upright = false;
	}
	else {
		upright = true;
	}

	if (upright != oldUpright) {//Has changed state
		Message toMotors;

		if (upright) {//has been put the right way up
			toMotors.set(STD, thisDevice, MOTORS, PARAM1, 255, 255, 1); //vehicle upright
		}
		else {//has fallen over
			toMotors.set(STD, thisDevice, MOTORS, PARAM1, 0, 0, 1); //rollover detected
		}
		AKB0.cancel(toMotors);//cancel previous ACK waiting
		AKB0.add(toMotors); //add message to acknowledge message send list
	}
	oldUpright = upright;
}

