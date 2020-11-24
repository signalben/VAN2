//https://www.hobbytronics.co.uk/arduino-timer-interrupts
//#include "C:\Users\Ben\AppData\Local\Arduino15\packages\esp32\hardware\esp32\1.0.4\libraries\WiFi\src\WiFi.h"
//https://techtutorialsx.com/2019/10/20/esp32-getting-started-with-esp-now/
#include "VANESP.h"
#include <esp_now.h>
#include <WiFi.h>
uint8_t inbyte;
int activeSerial = 0;
uint16_t mask = B11111111;
esp_now_peer_info_t peerInfo;
uint8_t ringBuff[10][5];
int readFrom = 0;
bool ringBuffAvailable = false;


void ringBufferStore(uint8_t[7] data) {

	for (int i = 0; i < 10; i++) {

		if (ringBuff[i][0] == 0) {
			memcpy(ringBuff[i][0], data +1, 5);
			ringBuffOverload = false;
			ringBuffAvailable = true;
			return;
		}
	}
		Serial.println("ESP-NOW ring buffer over capacity");
		return;
}

void ringBufferRead() {

	for (int i = readFrom; i < 10; i++) {

		if (ringBuff[i][0] != 0) {
			memcpy(message[0], ringBuff[i][0], 5);
			for (int j = 0; j < 5; j++) {
				ringBuff[i][j] = 0;
			}

			if (readFrom >= 9) {
				readFrom = 0;
			}
			else {
				readFrom++;
			}
			return;
		}
	}
		ringBuffAvailable = false;
		return;
	}


flush the buffer before compile
void ringBufferFlush(data) {
	memset(ringBuff, 0, sizeof(ringBuff));
	ringBuffAvailable = false;
}

void onReceiveData(const uint8_t* mac, const uint8_t* data, int len) {

	if (len != 7) {
		Serial.println("ESP-NOW message wrong length");
		return;
	}

	if (data[0] != 0x07) or (data[6] != 0x00) {
		Serial.println("ESP-NOW message start or end invalid");
		return;
	}

	ringBufferStore(data);
	espMsgAvailable = true;
}

void espNowSetup() {
	WiFi.mode(WIFI_STA);
	if (esp_now_init() != ESP_OK) {
		Serial.println("Error initializing ESP-NOW");
		return;
	}
	esp_now_register_recv_cb(onReceiveData);
	memcpy(peerInfo.peer_addr, peerMAC, 6);
	peerInfo.channel = 0;
	peerInfo.encrypt = false;
	if (esp_now_add_peer(&peerInfo) != ESP_OK) {
		Serial.println("Failed to add peer");
		return;
	}
}

void esp_send() {
	//message: start, source, dest, CMD, DAT, DAT, END
	uint8_t message[7] = { 0x07, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00 };
	esp_err_t result = esp_now_send(masterAddress, (uint8_t*)&message, 7);
	if (result == ESP_OK) {
		//Serial.println("Sent with success");
	}
	else {
		Serial.println("Error sending the data");
	}
}

void activeSerialWrite(uint8_t outbyte) {
	if (activeSerial == 0) {
		Serial.write(outbyte);
	}
	else if (activeSerial == 1) {
		Serial1.write(outbyte);
	}
	else if (activeSerial == 2) {
		Serial2.write(outbyte);
	}
	else if (activeSerial == 3) { //This is ESP NOW	
		Serial.printLn("Error- activeSerial 3 is for ESP NOW");
	}
	else {
		Serial.write(outbyte);
	}

}

int activeSerialAvailable() {

	if (activeSerial == 0) {
		return Serial.available();
	}
	if (activeSerial == 1) {
		//Serial.print("Checking Serial1 avialable: ");
		//Serial.println(Serial1.available());
		return Serial1.available();
	}
	if (activeSerial == 2) {
		return Serial2.available();
	}
	if (activeSerial == 3) {
		Serial.printLn("Error- activeSerial 3 is for ESP NOW");
		return 0;
	}
	else {
		return Serial.available();
	}

}

uint8_t activeSerialRead() {
	if (activeSerial == 0) {
		return Serial.read();
	}
	if (activeSerial == 1) {
		return Serial1.read();
	}
	if (activeSerial == 2) {
		return Serial2.read();
	}
	if (activeSerial == 3) {
		Serial.printLn("Error- activeSerial 3 is for ESP NOW");
		return '0';
	}
	else {
		return Serial.read();
	}
}

void setTimer() {

	timer = timerBegin(0, 80, true);
	timerAttachInterrupt(timer, &onTimer, true);
	timerAlarmWrite(timer, 10000, true);
	timerAlarmEnable(timer);
}

void wifiSetup() {


	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(1000);
		Serial.println("Connecting to WiFi..");
	}

	Serial.println("Connected to the WiFi network");
	Serial.println(WiFi.localIP());
	wifiServer.begin();
	Serial.println("Waiting for connection");

	while (true) {
		client = wifiServer.available();
		if (client.connected()) {
			break;
		}
	}
	Serial.println("Client connected");

}

bool getByte() {

	//Serial.print("Active serial:");
	//Serial.println(activeSerial);

	if (activeSerialAvailable() == 0) {
		//Serial.println("Buffer empty");
		return false;
	}

	inbyte = activeSerialRead();
	//Serial.println("Byte");
	return true;
}

bool getStart() {
	for (int i = 0; i < 4; i++) {
		if (getByte()) {

			if (inbyte == '\a') {
				//Serial.println("START");
				return true;
			}
		}
		else {
			return false;
		}
	}
	return false;
	Serial.println("NO START");
}

void getMessage(int selected) {
	activeSerial = selected;
	messageValid = false;
	uint8_t buff[5];

	if (activeSerial == 3) {
		if (espMsgAvailable) and ()
			drhhedjsjsj
			handleMessage();
	}

	else {
		if (getStart()) {
			// Serial.println("START");


			for (int i = 0; i < 5; i++) {
				if (getByte()) {
					// Serial.println(inbyte);
					buff[i] = inbyte;
				}
			}

			if (getByte()) {
				if (inbyte == '\n') {
					//  Serial.println("END");
					messageValid = true;
				}
			}
			if (messageValid) {
				for (int i = 0; i < 5; i++) {
					message[i] = buff[i];
				}
			}
		}
		handleMessage();
	}
}

void handleMessage() {
	int i = 0;

	if (messageValid) {

		for (i = 0; i < (0 + n_lobject); i++) {
			if (lobject[i] == message[1]) {
				commandList();
				return;
			}
		}

		for (i = 0; i < n_port0; i++) {
			if (port0[i] == message[1]) {
				activeSerial = 0;
				passMessage();
				return;
			}
		}

		for (i = 0; i < n_port1; i++) {
			if (port1[i] == message[1]) {
				activeSerial = 1;
				passMessage();
				return;
			}
		}

		Serial.print("ERROR- Message has destination unknown to this node:");
		Serial.println(message[1], HEX);
		messageValid = false;
	}
}

void debugMessage() {
	Serial.print(message[0], HEX);
	Serial.print("_");
	Serial.print(message[1], HEX);
	Serial.print("_");
	Serial.print(message[2], HEX);
	Serial.print("_");
	Serial.print(message[3], HEX);
	Serial.print("_");
	Serial.println(message[4], HEX);
}

void passMessage() {
	activeSerialWrite(START);
	for (int i = 0; i < 5; i++) {
		activeSerialWrite(message[i]);
	}
	activeSerialWrite(END);
	messageValid = false;
	return;
}


