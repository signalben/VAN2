#include "van_dev_remote.h"
#include "Arduino.h"
#include <message.h>

//Requires only an address to initialize, however this device must call a main function IMU0Update() to operate
van_rem::van_rem(uint8_t deviceAdress) {
	thisDevice = deviceAdress; //IMU
	IRReadValue = 0;	//Keeps new remote code received
	lastReport = 0;		//previous time of reporting
	period = 0;
}

void van_rem::command(message inData) {

	if (inData.cmd == PERIOD) { //set period
		period = inData.getDataInt();
		return;
	}

	if (inData.cmd == SETDEST) { //set destination of reports
		destination = inData.dat1;
		return;
	}

	if (inData.cmd == REPORTNOW) { //one off report
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

	const unsigned long left = 16716015, right = 16734885, up = 16718055, down = 16730805, repeat = 4294967295; //Constant codes sent by the remote for button pushes, and button hold down
	static unsigned long previousIRRead; //Allows repeat codes to be understood
	static bool stopped; //True when remote is not in use
	int lVel, rVel;	//Holds commanded velocities as determined by remote 
	int scaleVel = 127; //Sets magnetude of remote velocity commands (127 is maximum)
	
	REM0Update();//External function (main), returns any new codes, or zero if none
	//Construct message with L,R velocites of zero
	Message buff;
	buff.set(STD, thisDevice, destination, SET, 128, 128, 1);

	if (IRReadValue) {//If button push, or holdown
		AKB0.cancel(buff);//cancel previous ACK waiting
		stopped = false;	//If code has been received, device is in use
		if (IRReadValue == repeat) {//Replace a repeat code with the last button code received
			IRReadValue = previousIRRead;
		}
		switch (IRReadValue) {//Set target motor velocities based which button pushed 
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
			//Default to stationary if code corrupt, or unused button pushed
		default:
			lVel = 0;
			rVel = 0;
			break;
		}

		previousIRRead = IRReadValue; //Store last button push for next call
		lVel = (lVel * scaleVel) + 128; //Scale to specified magnetude, convert range to 0-255 for transmission to PID
		rVel = (rVel * scaleVel) + 128;
		lVel = constrain(lVel, 0, 255);//Ensure values suitable for 8 bit transmission
		rVel = constrain(rVel, 0, 255);

		//Set add velocities to message before sending
		buff.set(STD, thisDevice, destination, SET, uint8_t(lVel), uint8_t(rVel), 1);
		handleMessage(buff);
	}
	else if (stopped == false) {//No code received, but device has not yet stopped
			stopped = true;//Register that device has stopped
			buff.set(STD, thisDevice, destination, SET, 128, 128, 1);//Construct message with stationary velocities
			AKB0.add(buff); //add message to acknowledge message send list, the message will be repated until the destination (PID) node has verified its recpetion
	}


	lastReport = millis();
}
