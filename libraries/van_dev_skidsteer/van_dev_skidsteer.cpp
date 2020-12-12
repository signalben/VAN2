//https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/

#include "van_dev_skidsteer.h"
#include "Arduino.h"
#include <message.h>

van_skid::van_skid(uint8_t deviceAdress) {
	thisDevice = deviceAdress; //Skidsteer
	destination = 0; //test to pc, should be to motors in use 
}

void van_skid::command(message inData) {

	if (destination == 0) {
		return;
	}

	if (inData.cmd == SET) {

		int velocity = inData.dat0 - 128;
		int heading = inData.dat1 - 128;//range -128, 127
		int left, right;

		right = velocity + heading;
		left =  velocity -heading;

		left = left + 128;// range 0,255
		right = right + 128;

		left  = constrain(left, 0, 255);
		right = constrain(right, 0, 255);

		Message buff;
		buff.set(thisDevice, destination, SET, uint8_t(left), uint8_t(right), 1);
		handleMessage(buff);

		return;
	}

	if (inData.cmd == SETDEST) {
		destination = inData.dat1;
	}
}


