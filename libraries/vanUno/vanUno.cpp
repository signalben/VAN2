//https://techtutorialsx.com/2019/10/20/esp32-getting-started-with-esp-now/
//#include "Arduino.h"
#include "vanUno.h"

void Vlog(char inData[]) {
	if (verboseLog == true) {
		Serial.println(inData);
	}
}

void Elog(char inData[]) {
	if (errorLog == true) {
		Serial.println(inData);
	}
}

void vanWrite(uint8_t outbyte) {
	if (vanSerial == 0) {
		Serial.write(outbyte);
		return;
	}
	Elog("ERROR- Write to nonexistant serial port");
}

int vanAvailable() {
	if (vanSerial == 0) {
		return Serial.available();
	}
	Serial.println("ERROR- Checking buffer of nonexistant serial port");
	return 0;
}

uint8_t vanRead() {
	if (vanSerial == 0) {
		return Serial.read();
	}
	Serial.println("ERROR- Reading nonexistant serial port");
	return 0;
}

bool getStart() {
	while (vanAvailable() >= 7) {//While there are enough bytes for a complete message
		if (vanRead() == START) {//Message start char detected
			return true;
		}
	}
	return false;
}

Message getMessage(int selected) {
	vanSerial = selected;
	Message buff;
	buff.valid = false;

	if (getStart() == false) {
		return buff; //Returning message to ignore, as .valid = false
	}

	buff.src = vanRead();
	buff.dest = vanRead();
	buff.cmd = vanRead();
	buff.dat0 = vanRead();
	buff.dat1 = vanRead();
	if (vanRead() == END) {
		buff.valid = true;
	}
	return buff;
}

void handleMessage(Message inData) {


	if (inData.valid == false) {
		return;
	}

	int i = 0;
	for (i = 0; i < (0 + N_LOCALDEVICE); i++) { //Check if message destination is a local device
		if (LOCALDEVICE[i] == inData.dest) {
			commandList(inData);  //If it is then pass message to the command list
			return;
		}
	}

	for (i = 0; i < N_SPORT0; i++) {   //Check if message destination is via port 0
		if (SPORT0[i] == inData.dest) {
			vanSerial = 0;
			sendMessage(inData);
			return;
		}
	}

	Elog("ERROR- Message has destination unknown to this node:");
}

void sendMessage(Message inData) {
	vanWrite(START);
	vanWrite(inData.src);
	vanWrite(inData.dest);
	vanWrite(inData.cmd);
	vanWrite(inData.dat0);
	vanWrite(inData.dat1);
	vanWrite(END);
	return;
}

void showMessage(Message inData) {
	Serial.print(inData.src);
	Serial.print("_");
	Serial.print(inData.dest);
	Serial.print("_");
	Serial.print(inData.cmd);
	Serial.print("_");
	Serial.print(inData.dat0);
	Serial.print("_");
	Serial.println(inData.dat1);
}

