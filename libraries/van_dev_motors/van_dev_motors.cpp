//https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/

#include "van_dev_motors.h"
#include "Arduino.h"
#include <message.h>

van_motors::van_motors(uint8_t deviceAdress, int a, int b, int c, int d) {

	aDirPin = a; // 12
	aPwmPin = b; //  3
	bDirPin = c; // 13
	bPwmPin = d; // 11

	pinMode(aDirPin, OUTPUT);
	pinMode(aPwmPin, OUTPUT);
	pinMode(bDirPin, OUTPUT);
	pinMode(bPwmPin, OUTPUT);

	thisDevice = deviceAdress; //MOTORS
	destination = 0; //test to pc, should be 0 
	lastReport = 0;
	period = 0;
	safeAhead = true;
}

void van_motors::command(message inData) {
	showMessage(inData);
	Serial.println(safeAhead);
	Serial.println(upright);
	Serial.println();

	if (inData.cmd == SET) {

		if (upright) {
			int left = inData.dat1 - 128;//dat0, dat1  motor a, b range -128,127
			int right = inData.dat0 - 128;

			if (left > 0) {

				digitalWrite(aDirPin, LOW);//f

				if (safeAhead)
					analogWrite(aPwmPin, (2 * left)); //range 0,255	
				else
					analogWrite(aPwmPin, 0); //range 0,255
			}
			else {
				digitalWrite(aDirPin, HIGH);//r
				analogWrite(aPwmPin, (2 * -left));
			}

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

		else {
			analogWrite(aPwmPin, 0);
			analogWrite(aPwmPin, 0);
		}
	}

	if (inData.cmd == PARAM0) {
		if (inData.dat1 > 0)
			safeAhead = true;

		else
			safeAhead = false;

	}

	if (inData.cmd == PARAM1) {
		if (inData.dat1 > 0)
			upright = true;

		else
			upright = false;
	}

	//Motors currently only act on SET command, reporting code left for future functions (error reporting)

	if (inData.cmd == PERIOD) {
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

}



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

void van_motors::instantReport() {

	if (destination == 0) {
		return;
	}

	lastReport = millis();
}

