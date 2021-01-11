#include "van_dev_ultrasound.h"
#include "Arduino.h"
#include <message.h>

//Construct with address, trigger pin, echo pin
van_us::van_us(uint8_t deviceAdress, int a, int b) {

	trigPin = a;    // Trigger 11, pin numbers as used on buggy
	echoPin = b;    // Echo 12
	thisDevice = deviceAdress; //ULTRASOUND
	destination = 0; //Initially set in main as PC
	lastReport = 0;
	period = 0;
	obstructed = 1500;//Default obsticle detection value
}

void van_us::command(message inData) {

	if (inData.cmd == PERIOD) {			//Set period of reporting
		period = inData.getDataInt();
		return;
	}

	if (inData.cmd == SETDEST) {	//Change report destination
		destination = inData.dat1;
		return;
	}

	if (inData.cmd == REPORTNOW) {	//give one-off report immeadiatly
		if (inData.dat1 != 0) {
			uint8_t suspendedDesination = destination;
			destination = inData.dat1;
			instantReport();
			destination = suspendedDesination;
		}
		else {
			instantReport();
		}
		return;
	}

	if (inData.cmd == PARAM0) {	//Allows run-time mofication block obsticle distance
		obstructed = inData.getDataInt();
	}

}

void van_us::autoReport() {

	if (period == 0) { //Disables device- Note: disables obstruction messages to motor as well as reporting 
		return;
	}

	unsigned long now = millis(); //allows changing of report period
	if ((now - lastReport) < period) {
		return;
	}

	instantReport();
}

void van_us::instantReport() {

	if (destination == 0) { //Disables device- Note: disables obstruction messages to motor as well as reporting 
		return;
	}

	pinMode(trigPin, OUTPUT);	//sets triggerpin as output
	digitalWrite(trigPin, LOW);	//toggling triggerpin promts SR04 sensor to send a pulse 
	delayMicroseconds(5);
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin, LOW);
	pinMode(echoPin, INPUT);	//echopin set as input
	int distance = int(pulseIn(echoPin, HIGH));//pulsIn times delay until SRO4 detects its own reflected pulse, in uS

	//Constructs and sends message with obsticle distance as in16, in uS return delay
	Message buff;
	buff.setInt(STD, thisDevice, destination, REPORT, distance, 1);
	handleMessage(buff);
	
	//runs everytime device is called 
	assessDanger(distance);

	lastReport = millis();
}

//function determines if pulse return delay is short enough to indicate an object is blocking forward movement of buggy
void van_us::assessDanger(int distance) {
	bool clearAhead;
	static bool oldClearAhead = false;	//Stores old state between calls to detect state change

	if (distance < obstructed) {
		clearAhead = false;
	}
	else {
		clearAhead = true;
	}

	if (clearAhead != oldClearAhead) {//Has changed state
		Message toMotors;

		if (clearAhead) {//obsticle cleared
			toMotors.set(STD, thisDevice, MOTORS, PARAM0, 255, 255, 1); //clear ahead
		}
		else {//obsticle detected
			toMotors.set(STD, thisDevice, MOTORS, PARAM0, 0, 0, 1); //obsticle detected
		}
		AKB0.cancel(toMotors);//cancel previous ACK waiting
		AKB0.add(toMotors); //add message to acknowledge message send list
	}
	oldClearAhead = clearAhead;//Store current state for next call
}

