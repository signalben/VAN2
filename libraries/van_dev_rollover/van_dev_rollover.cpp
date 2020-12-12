
#include "van_dev_rollover.h"
#include "Arduino.h"
#include <message.h>

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

	Message enableMotors;
	
	if ((angle < -45) || (angle > 45)) {
		enableMotors.set(thisDevice, destination, PARAM1, 0, 0, 1); //rollover detected
	}
	else {
		enableMotors.set(thisDevice, destination, PARAM1, 255, 255, 1); //vehicle upright
	}
	handleMessage(enableMotors);
	//showMessage(enableMotors);*/
}

