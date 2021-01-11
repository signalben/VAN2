#include "van_dev_motors.h"
#include "Arduino.h"
#include <message.h>

//Construction requires an address, pins for motor A direction, motor A PWM, motor B direction, motor B PWM
van_motors::van_motors(uint8_t deviceAdress, int a, int b, int c, int d) {

	aDirPin = a; // 12 pins as used on buggy
	aPwmPin = b; //  3
	bDirPin = c; // 13
	bPwmPin = d; // 11

	//All pins set as outputs
	pinMode(aDirPin, OUTPUT);
	pinMode(aPwmPin, OUTPUT);
	pinMode(bDirPin, OUTPUT);
	pinMode(bPwmPin, OUTPUT);

	thisDevice = deviceAdress; //MOTORS
	destination = 0; //Initially set in main to PC, for reporting enable/disable state
	lastReport = 0;
	period = 0;
	safeAhead = true;
}

void van_motors::command(message inData) {

	//Uncomment 2 lines below to enable motors regardless of ultrasound objects, roll angle
	//safeAhead = true;
	//upright = true;

	if (inData.cmd == SET) { //received message to set PWM, direction on both channels	
		//Messages with command SET use DAT0, DAT1 to set left, right target velocities from a single message.
		//Input target velocites range from 0 to 128 to 255, respectivly full reverse, stop, full forwards
		if (upright) {//buggy has not rolled over
			int left = inData.dat1 - 128;//convert both motor inputs to range -128, 127
			int right = inData.dat0 - 128;

			if (left > 0) {	//If above 0, target velocity is forwards

				digitalWrite(aDirPin, LOW);//Set direction pin to forwards

				if (safeAhead)//Only allow forwards motion if it is safe ahead
					analogWrite(aPwmPin, (2 * left)); //multiplied by 2 to use full PWM range 0,255	
				else
					analogWrite(aPwmPin, 0); //If an obsticle is ahead, set forward motion to 0
			}
			else {	//If below 0, target velocity is backwards. No need to check if it is safe ahead.
				digitalWrite(aDirPin, HIGH);//Set direction to reverse 
				analogWrite(aPwmPin, (2 * -left)); //Invert value to be positive for setting PWM 
			}

			//Identical logic for other motor
			if (right > 0) {
				digitalWrite(bDirPin, HIGH);//f

				if (safeAhead)
					analogWrite(bPwmPin, (2 * right));
				else
					analogWrite(bPwmPin, 0);
			}
			else {
				digitalWrite(bDirPin, LOW);//r
				analogWrite(bPwmPin, (2 * -right));
			}

		}

		else {//If not upright, buggy has rolled over, set both PWM values to 0 
			analogWrite(aPwmPin, 0);
			analogWrite(bPwmPin, 0);
		}
	}

	if (inData.cmd == PARAM0) {//Mesages recieved as cmd PARAM0 (from ultrasound) indicate introduction or removal of a blocking object 
		if (inData.dat1 > 0)	//Data values are 255 255 for object removed or 0, 0 for obsticle detected
			safeAhead = true;	//Bool safeAhead retains state between calls

		else
			safeAhead = false;

		instantReport();	//Report this change to the PC

	}

	if (inData.cmd == PARAM1) { //Mesages recieved as cmd PARAM1, are from device rollover, based on IMU Y axis angle
		if (inData.dat1 > 0)	//Indicates change of state from upright to rolled over or vice-versa
			upright = true;		//Same message data formats as with ultrasound

		else
			upright = false;

		instantReport(); //Report this change to the PC
	}

	//Period left set to 0 in use, as continous reporting of the motors enable/disabled state is not required
	//since PWM, direction pins are set immeadiatly upon recieving commands
	if (inData.cmd == PERIOD) {
		period = inData.getDataInt();
		return;
	}

	if (inData.cmd == SETDEST) {
		destination = inData.dat1;
		return;
	}

	if (inData.cmd == REPORTNOW) { //give one-off report, to specified destination, or normal dest if none specified
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

//Code left in to allow continous reporting if desired
void van_motors::autoReport() {

	if (period == 0) {
		return;
	}

	unsigned long now = millis();
	if ((now - lastReport) < period) {
		return;
	}

	instantReport();
}

//Used for reporting motor enable/disable status to PC
void van_motors::instantReport() {

	if (destination == 0) { //Can be disabled by setting destination to 0
		return;
	}

	//Construct and send message to PC. dat0, contains safeAhead status as 0 or 1, and dat1 contains upright status
	Message buff;
	buff.set(STD, thisDevice, destination, REPORT, safeAhead, upright, 1);
	handleMessage(buff);

	lastReport = millis();
}

