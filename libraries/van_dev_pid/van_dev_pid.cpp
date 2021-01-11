#include "van_dev_pid.h"
#include "Arduino.h"
#include <message.h>

int encGain = 10, Pg = 15, Ig = 0, Dg = 2;//Default PID gains, giving good control of buggy
//Variables keep recieved target velocities, positional error, current velocities, and previous velocity error (needed to calculate change of velocity error)
int lTarget = 128, rTarget = 128, lPosE = 0, lOldVelE = 0, lVel = 0, rPosE = 0, rOldVelE = 0, rVel = 0;

//Variables used only for testing of motor/PID responses
//int loopCount = 0;
//int loopVel = 0;

//functions.isrLeft and .isrRight must be called by interupt service routines setup in main sketch.
void van_pid::isrLeft() {
	//Arduino Nano only has 2 interupt pins, one for each encoder. Therefore ISRs only triggered upon change of one encoder channel: trigPin
	//Direction of rotation determined by reading both channels, and either incrementing or decrementing a count
	//Count used as a measure of velocity, since it is reset to 0 at regular time intervals 
	if (digitalRead(lTrigPin)) { 
		if (digitalRead(lDirPin))
			lVel++;
		else
			lVel--;
	}

	else {
		if (digitalRead(lDirPin))
			lVel--;
		else
			lVel++;
	}
}

//Function identical to isrLeft, other than reading other encoder, storing a seperate count
void van_pid::isrRight() {
	if (digitalRead(rTrigPin)) {
		if (digitalRead(rDirPin))
			rVel--;
		else
			rVel++;
	}

	else {
		if (digitalRead(rDirPin))
			rVel++;
		else
			rVel--;
	}
}

//Construct PID object, requires PID address, pins in order: right trigger, right direction, left trigger, left direction
//both trigger pins must be interupt capable
van_pid::van_pid(uint8_t deviceAdress, int a, int b, int c, int d) {

	thisDevice = deviceAdress; //assigns address to this device
	destination = 0; //destination should be motors, set in main sketch
	lastReport = 0;
	period = 0;

	rTrigPin = a; //2 These pin values used on buggy
	rDirPin = b; // 4
	lTrigPin = c; // 3
	lDirPin = d; //5

	//They must all be inputs:
	pinMode(lTrigPin, INPUT);
	pinMode(lDirPin, INPUT);
	pinMode(rTrigPin, INPUT);
	pinMode(rDirPin, INPUT);
}

void van_pid::command(message inData) {
	//Messages with command SET use DAT0, DAT1 to set left, right target velocities from a single message.
	//Input target velocites range from 0 to 128 to 255, respectivly full reverse, stop, full forwards.
	if (inData.cmd == SET) { 
		lTarget = inData.dat0 - 128;//This range is changed to -128 to 255, to be used in PID calcs
		rTarget = inData.dat1 - 128;
		return;
	}

	if (inData.cmd == PERIOD) {//In this case 20ms was found to be suitable, and is initially set in main
		period = inData.getDataInt();
		return;
	}

	if (inData.cmd == SETDEST) { //set PID dest
		destination = inData.dat1;
		Message buff;				//send message to new destination device confirming change
		buff.set(STD, thisDevice, destination, RESPONSE, 0, destination, 1);
		handleMessage(buff);
		return;
	}

	//Messages with cmd PARAM0,1,2,3 modify encoderGain, PI and D values (max 255) 
	if (inData.cmd == PARAM0) {
		encGain = inData.dat1;
		return;
	}

	if (inData.cmd == PARAM1) {
		Pg = inData.dat1;
		return;
	}

	if (inData.cmd == PARAM2) {
		Ig = inData.dat1;
		return;
	}

	if (inData.cmd == PARAM3) {
		Dg = inData.dat1;
		return;
	}

}

//Called repeatedly from main, allows PID to regularly send motor commands
void van_pid::autoReport() {

	if (period == 0||destination == 0) {//Allows PID to be disabled, disables by default if no destination
		return;
	}

	unsigned long now = millis();
	if ((now - lastReport) < period) {
		return;
	}

	instantReport();

	lastReport = millis();
}

void van_pid::instantReport() {

	//Used only for testing motor and PID responses, reports left encoder counts over 0.5s, when pid.period = 20
	/*loopCount++;
	loopVel += lVel;
	if (loopCount == 25) {
		loopCount = 0;
		Message Lencread;
		Lencread.setInt(STD, thisDevice, MEGA, REPORT, loopVel, 1);
		handleMessage(Lencread);
		loopVel = 0;
	}*/
	
	//Calculations to produce left motor command
	int lVelE = lTarget - lVel*encGain; //proportional error
	lPosE += lVelE; //update integral/positional error
	lPosE = constrain(lPosE, -1024, 1024); //prevent too much integral windup
	int lAccE = lVelE - lOldVelE;	//derivative/acceleration error
	int left = ((Pg * lVelE) + (Ig * lPosE) + (Dg * lAccE)) / 16; //Sum errors, weighted by their gains, scale down to 8 bit range
	lOldVelE = lVelE; //Store for use in next call

	//Calculations to produce right motor command
	int rVelE = rTarget - rVel * encGain; //proportional error
	rPosE += rVelE; //update integral/positional error
	rPosE = constrain(rPosE, -1024, 1024); //prevent too much integral windup
	int rAccE = rVelE - rOldVelE; //derivative/acceleration error
	int right = ((Pg * rVelE) + (Ig * rPosE) + (Dg * rAccE)) / 16; //Sum errors, weighted by their gains, scale down to 8 bit range
	rOldVelE = rVelE; //Store for use in next call

	//Reset to accumulate next period of encoder counts
	lVel = 0; 
	rVel = 0;

	//Limit to sensible values
	rOldVelE = constrain(rOldVelE,  -128, 127);
	lOldVelE = constrain(lOldVelE, -128, 127);

	//Convert output to range 0,255 for transmission
	left = left + 128;
	right = right + 128;

	//Garentee values can be stored in a byte 
	left = constrain(left, 0, 255);
	right = constrain(right, 0, 255);

	//Construct and send message updating motor PWM values
	Message buff;
	buff.set(STD, thisDevice, destination, SET, uint8_t(left), uint8_t(right), 1);
	handleMessage(buff);
}



