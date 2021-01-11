//https://www.instructables.com/Connecting-MPU6050-With-ESP32/
//REF MPU_6050 example PDF

#include "van_dev_imu.h"
#include "Arduino.h"
#include <message.h>



van_imu::van_imu(uint8_t deviceAdress) {
	thisDevice = deviceAdress; //Should be given IMU contant from netDef
	AYdest = 0;
	GZdest = 0;
	ANGYdest = 0;
	lastReport = 0;
	period = 0;
}

void van_imu::command(message inData) {

	if (inData.cmd == PERIOD) {			//sends period of measurement/sending for all channels 
		period = inData.getDataInt();
		return;
	}

	//Commands PARAM0, 1, 2 set destination of sensor channels independantly.
	if (inData.cmd == SETDEST) {

		if (inData.dat0 == PARAM0) {
			AYdest = inData.dat1;
			return;
		}
		if (inData.dat0 == PARAM1) {
			GZdest = inData.dat1;
			return;
		}
		if (inData.dat0 == PARAM2) {
			ANGYdest = inData.dat1;
			return;
		}
		return;
	}

	//Can be promted to give one-off reading
	if (inData.cmd == REPORTNOW) {
		instantReport();
		return;
	}

}

void van_imu::autoReport() {

	//allows reading/sending to be turned off
	if (period == 0) {
		return;
	}

	unsigned long now = millis();
	if ((now - lastReport) < period) {
		return;
	}

	instantReport();
}

void van_imu::instantReport() {

	IMU0Update();//function in main sketch, updates AY,GZ,ANGY
	sendChannel(AY, PARAM0, AYdest, 1000);//forward velocity
	sendChannel(GZ, PARAM1, GZdest, 1);//turn velocity
	sendChannel(ANGY, PARAM4, ANGYdest, 1);//rollover
	
	lastReport = millis();
}

//function contructs and handles messages containing readings, must be given destination and scale
void van_imu::sendChannel(float reading, uint8_t channel, uint8_t destination, int scale) {
	//dest = 0 allows individual channels to be disabled
	if (destination == 0) {
		return;
	}
	float scaledReading = reading * scale;
	int intReading = int(scaledReading);//scaling requiring to send data provided as a float 

	Message buff;
	buff.setInt(STD, thisDevice, destination, channel, intReading, 1);//channel specified using command byte 
	handleMessage(buff);
}