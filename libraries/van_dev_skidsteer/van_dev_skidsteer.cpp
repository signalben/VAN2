//https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/

#include "van_dev_skidsteer.h"
#include "Arduino.h"
#include <message.h>
#include "ackbuff.h"

//Requires only its unique address to construct, no pins are used
van_skid::van_skid(uint8_t deviceAdress) {
	thisDevice = deviceAdress; //Skidsteer
	destination = 0; //set in main to PID
}

void van_skid::command(message inData) {

	if (destination == 0) {// allows disabling
		return;
	}

	if (inData.cmd == SET) {

		int velocity = inData.dat0 - 128;
		int heading = inData.dat1 - 128;//Converts received readings in 0,255 range to -128, 127
		int left, right;

		right = velocity + heading;//Produce left, right target velocities, with a difference produced by a non-zero heading
		left =  velocity -heading;

		left = left + 128;//Convert back to range 0,255
		right = right + 128;

		left  = constrain(left, 0, 255);//Ensure values are suitable for transmission as single byte
		right = constrain(right, 0, 255);

		//Construct and send message containing L,R targets to PID
		Message buff;
		buff.set(STD, thisDevice, destination, SET, uint8_t(left), uint8_t(right), 1);
		handleMessage(buff);

		return;
	}

	if (inData.cmd == SETDEST) {
		destination = inData.dat1;
	}
}


