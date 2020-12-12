//https://www.instructables.com/Connecting-MPU6050-With-ESP32/
//REF MPU_6050 example PDF

#include "van_dev_imu.h"
#include "Arduino.h"
#include <message.h>

van_imu::van_imu(uint8_t deviceAdress) {
	thisDevice = deviceAdress; //IMU
	AXdest = 0;
	AYdest = 0;
	AZdest = 0;
	ANGXdest = 0;
	ANGYdest = 0;
	ANGZdest = 0;
	lastReport = 0;
	period = 0;
}

void van_imu::command(message inData) {

	if (inData.cmd == PERIOD) {
		period = inData.getDataInt();
		return;
	}

	if (inData.cmd == SETDEST) {

		if (inData.dat0 == PARAM0) {
			AXdest = inData.dat1;
			return;
		}
		if (inData.dat0 == PARAM1) {
			AYdest = inData.dat1;
			return;
		}
		if (inData.dat0 == PARAM2) {
			AZdest = inData.dat1;
			return;
		}
		if (inData.dat0 == PARAM3) {
			ANGXdest = inData.dat1;
			return;
		}
		if (inData.dat0 == PARAM4) {
			ANGYdest = inData.dat1;
			return;
		}
		if (inData.dat0 == PARAM5) {
			ANGZdest = inData.dat1;
			return;
		}
		return;
	}

	if (inData.cmd == REPORTNOW) {
		instantReport();
		return;
	}

}

void van_imu::autoReport() {

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

	IMU0Update();
	sendChannel(AX, PARAM0, AXdest, 1000);
	sendChannel(AY, PARAM1, AYdest, 1000);
	sendChannel(AZ, PARAM2, AZdest, 1000);
	sendChannel(ANGX, PARAM3, ANGXdest, 1);
	sendChannel(ANGY, PARAM4, ANGYdest, 1);
	sendChannel(ANGZ, PARAM5, ANGZdest, 1);

	lastReport = millis();
}

void van_imu::sendChannel(float reading, uint8_t channel, uint8_t destination, int scale) {
	if (destination == 0) {
		return;
	}
	float scaledReading = reading * scale;
	int intReading = int(scaledReading);

	Message buff;
	buff.setInt(thisDevice, destination, channel, intReading, 1);
	handleMessage(buff);
	//showMessage(buff);
}