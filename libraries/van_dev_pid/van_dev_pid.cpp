//REF: http://www.4tronix.co.uk/arduino/sketches/initio_03.ino

#include "van_dev_pid.h"
#include "Arduino.h"
#include <message.h>

//int encGain = 10, Pg = 15, Ig = 0, Dg = 2;
int encGain = 10, Pg = 15, Ig = 0, Dg = 2;
int lTarget = 128, rTarget = 128, lPosE = 0, lOldVelE = 0, lVel = 0, rPosE = 0, rOldVelE = 0, rVel = 0;
int oldLTarget = 0, oldRTarget = 0;

void van_pid::isrLeft() {

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

van_pid::van_pid(uint8_t deviceAdress, int a, int b, int c, int d) {

	thisDevice = deviceAdress; //MOTORS
	destination = 0; //test to pc, should be 0 
	lastReport = 0;
	period = 0;

	rTrigPin = a; //2
	rDirPin = b; // 4
	lTrigPin = c; // 3
	lDirPin = d; //5

	pinMode(lTrigPin, INPUT);
	pinMode(lDirPin, INPUT);
	pinMode(rTrigPin, INPUT);
	pinMode(rDirPin, INPUT);
}

void van_pid::command(message inData) {

	if (inData.cmd == SET) { 
		lTarget = inData.dat0 - 128;
		rTarget = inData.dat1 - 128;
		return;
	}

	/*if ((inData.src == IMU)&&(inData.cmd == PARAM0)) {
		imuAcc = inData.getDataInt()-gravity;
		if (gravity == 0) {
			gravity = imuAcc;
		}
		return;
	}

	if ((inData.src == IMU) && (inData.cmd == PARAM1)) {
		//imuRV = inData.getDataInt();
		return;
	}*/

	if (inData.cmd == PERIOD) {
		period = inData.getDataInt();
		return;
	}

	if (inData.cmd == SETDEST) {
		destination = inData.dat1;
		return;
	}

	/*if (inData.cmd == PARAM0) {
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
	}*/

}

void van_pid::autoReport() {

	if (period == 0||destination == 0) {
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
	/*
	lTarget = (lTarget + oldLTarget) / 2;
	oldLTarget = lTarget;

	rTarget = (rTarget + oldRTarget) / 2;
	oldRTarget = rTarget;*/

	//Serial.println(lTarget);
	int lVelE = lTarget - lVel*encGain;
	lPosE += lVelE;
	lPosE = constrain(lPosE, -2046, 2045);
	int lAccE = lVelE - lOldVelE;
	int left = ((Pg * lVelE) + (Ig * lPosE) + (Dg * lAccE)) / 16;
	lOldVelE = lVelE;

	//Serial.println(left);

	int rVelE = rTarget - lVel * encGain;
	rPosE += rVelE;
	rPosE = constrain(rPosE, -2046, 2045);
	int rAccE = rVelE - rOldVelE;
	int right = ((Pg * rVelE) + (Ig * rPosE) + (Dg * rAccE)) / 16;
	rOldVelE = rVelE;

	lVel = 0;
	rVel = 0;

	//left = -left; ///MOTORS DIRECTION WRONG WAY ROUND?????

	left = left + 128;// range 0,255
	right = right + 128;

	left = constrain(left, 0, 255);
	right = constrain(right, 0, 255);

	Message buff;
	buff.set(STD, thisDevice, destination, SET, uint8_t(left), uint8_t(right), 1);
	//buff.set(thisDevice, destination, SET, uint8_t(left), 0, 1);
	handleMessage(buff);
	//Serial.println(buff.dat0);
}



