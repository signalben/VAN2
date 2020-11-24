//https://www.hobbytronics.co.uk/arduino-timer-interrupts
#include "VAN3.h"
//#include "C:\Users\Ben\AppData\Local\Arduino15\packages\esp32\hardware\esp32\1.0.4\libraries\WiFi\src\WiFi.h"
#include <WiFi.h>


uint8_t inbyte;
int activeSerial = 0;
uint16_t mask = B11111111;

const char* ssid = "TNCAP6C5D86";
const char* password = "F98D5947AD";
WiFiServer wifiServer(80);
WiFiClient client;

#if defined(__AVR_ATmega2560__)

int board_type = 1; //mega

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
	else if (activeSerial == 3) {
		Serial3.write(outbyte);
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
		return Serial3.available();
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
		return Serial3.read();
	}
	else {
		return Serial.read();
	}
}

void setTimer() {

	// initialize timer1
	noInterrupts();           // disable all interrupts
	TCCR1A = 0;
	TCCR1B = 0;

	// Set timer1_counter to the correct value for our interrupt interval
	//timer1_counter = 64911;   // preload timer 65536-16MHz/256/100Hz
	//timer1_counter = 64286;   // preload timer 65536-16MHz/256/50Hz
	//timer1_counter = 34286;   // preload timer 65536-16MHz/256/2Hz

	TCNT1 = timer1_counter;   // preload timer
	TCCR1B |= (1 << CS12);    // 256 prescaler
	TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
	interrupts();             // enable all interrupts
}

#endif

#if defined(ESP32)

int board_type = 2; //esp32

//https://techtutorialsx.com/2017/10/07/esp32-arduino-timer-interrupts/
hw_timer_t* timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

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
	else if (activeSerial == 3) { //This is WIFI		
		if (client.connected()) {
			client.write(outbyte);
			//Serial.print("Client connected");
		}
		else {
			//Serial.print("No client connected");
		}
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
			WiFiClient client = wifiServer.available();
			return	client.available();
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
			WiFiClient client = wifiServer.available();
			return	client.read();
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

#endif

#if defined (ARDUINO_AVR_DUEMILANOVE) 

	int board_type = 0; //uno

	void activeSerialWrite(uint8_t outbyte) {
		Serial.write(outbyte);
	}

	int activeSerialAvailable() {
		return Serial.available();
	}

	uint8_t activeSerialRead() {
		return Serial.read();
	}

	void setTimer() {

		// initialize timer1
		noInterrupts();           // disable all interrupts
		TCCR1A = 0;
		TCCR1B = 0;

		// Set timer1_counter to the correct value for our interrupt interval
		//timer1_counter = 64911;   // preload timer 65536-16MHz/256/100Hz
		//timer1_counter = 64286;   // preload timer 65536-16MHz/256/50Hz
		//timer1_counter = 34286;   // preload timer 65536-16MHz/256/2Hz

		TCNT1 = timer1_counter;   // preload timer
		TCCR1B |= (1 << CS12);    // 256 prescaler
		TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
		interrupts();             // enable all interrupts
	}

#endif

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


