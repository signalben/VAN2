#include "van_dev_joystick.h"
#include "Arduino.h"
#include <message.h>


//Construct joystick object, requires an address, pins in order: read analogue Y axis, read analogue X axis (range 0-4095 on ESP32)
van_js::van_js(uint8_t deviceAdress, int a, int b) {

	velPin = a;    // Velocity 36, pins used in this case
	hedPin = b;    // Heading  39
	thisDevice = deviceAdress; //JOYSTICK
	destination = 0; //initally set to skidsteer in main
	lastReport = 0;
	period = 0;
}

void van_js::command(message inData) {

	if (inData.cmd == PERIOD) { //set period of updates, 20ms default
		period = inData.getDataInt();
		return;
	}

	if (inData.cmd == SETDEST) {
		destination = inData.dat1;//Initially set to skidsteer in main
		return;
	}

	if (inData.cmd == REPORTNOW) {//give one-off report, to specified destination, or normal dest if none specified
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

//Called repeatedly from main
void van_js::autoReport() {

	if (period == 0) {//Allows disabling
		return;
	}

	unsigned long now = millis();
	if ((now - lastReport) < period) {
		return;
	}

	instantReport();
}

void van_js::instantReport() {

	if (destination == 0) {//Disabled if no destination set
		return;
	}

	int velocity = (analogRead(velPin)/16);	//range 0, 4095 => 0, 255
	int heading =  (analogRead(hedPin)/16);
	static int inUse = 0; //Likelyhood that read values are intential rather than noise, requires 3 sequential reads ouside of a deadband before readings are used 

	//105 to 135 is deadband 
	if ((105 < velocity) && (velocity < 135)) {
		velocity = 128; //if in deadband set to no motion, 128
	}
	else {
		inUse++; //if ouside deadband increase likelyhood this is intentional
	}

	//Identical logic for other axis
	if ((105 < heading) && (heading < 135)) {
		heading = 128;
	}
	else {
		inUse++;
	}

	 if (inUse >= 3) { //Enough readings outside deadband to conclude that the joystick is being used
		inUse = 3; //Prevent unbounded accumulation
		//Construct and send message containing read values for both axis. By default sent to skidsteer to convert to PID commands
		Message buff;
		buff.set(STD, thisDevice, destination, SET, uint8_t(velocity), uint8_t(heading), 1);
		handleMessage(buff);
	}
	 if ((velocity == 128) && (heading == 128)) { //joystick is in deadband, don't keep sending stationary messages- other device may be in control
		 inUse = 0; //reset, no longer in use
	 }

	lastReport = millis();
}

