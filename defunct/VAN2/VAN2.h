//https://www.hobbytronics.co.uk/arduino-timer-interrupts
//https://www.deviceplus.com/arduino/arduino-preprocessor-directives-tutorial/

#define VAN2_h
#include "Arduino.h"

uint8_t inbyte;
uint8_t buff[5];

bool messageValid;
int activeSerial = 0;
uint16_t mask = B11111111;
uint8_t  START = '\a';
uint8_t  END = '\n';
int timer1_counter;

void activeSerialWrite(uint8_t outbyte) {
	if (activeSerial == 0) {
		Serial.write(outbyte);
	}
#if defined(__AVR_ATmega2560__)
	else if (activeSerial == 1) {
		Serial1.write(outbyte);
	}
	else if (activeSerial == 2) {
		Serial2.write(outbyte);
	}
	else if (activeSerial == 3) {
		Serial3.write(outbyte);
	}
#endif
	else {
		Serial.write(outbyte);
}

}

int activeSerialAvailable() {
	if (activeSerial == 0) {
		return Serial.available();
	}
#if defined(__AVR_ATmega2560__)
	if (activeSerial == 1) {
		return Serial1.available();
	}
	if (activeSerial == 2) {
		return Serial2.available();
	}
	if (activeSerial == 3) {
		return Serial3.available();
	}
#endif
	else {
		return Serial.available();
	}

}

uint8_t activeSerialRead() {
	if (activeSerial == 0) {
		return Serial.read();
	}
#if defined(__AVR_ATmega2560__)
	if (activeSerial == 1) {
		return Serial1.read();
	}
	if (activeSerial == 2) {
		return Serial2.read();
	}
	if (activeSerial == 3) {
		return Serial3.read();
	}
#endif
	else {
		return Serial.read();
	}

}

bool getByte() {
	unsigned long start;
	unsigned long now;
	start = micros();

	//while (activeSerialAvailable() == 0) {
	//  now = micros();
	//  if ((now - start) > 10) {
	//     Serial.println("TIME OUT");
	//    return false;
	//  }
	//}

	if (activeSerialAvailable() == 0) {
		//Serial.println("Buffer empty");
		return false;
	}

	inbyte = activeSerialRead();
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

void passMessage() {
	activeSerialWrite(START);
	for (int i = 0; i < 5; i++) {
		activeSerialWrite(message[i]);
	}
	activeSerialWrite(END);
	messageValid = false;
	return;
}

void handleMessage() {
	int i = 0;

	if (messageValid) {

		for (i = 0; i < sizeof(lobject); i++) {
			if (lobject[i] == message[1]) {
				commandList();
				return;
			}
		}

		for (i = 0; i < sizeof(port0); i++) {
			if (port0[i] == message[1]) {
				activeSerial = 0;
				passMessage();
				return;
			}
		}

		for (i = 0; i < sizeof(port1); i++) {
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

void getMessage(int selected) {
	activeSerial = selected;
	messageValid = false;

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

void setTimer() {

	// initialize timer1
	noInterrupts();           // disable all interrupts
	TCCR1A = 0;
	TCCR1B = 0;

	// Set timer1_counter to the correct value for our interrupt interval
	timer1_counter = 64911;   // preload timer 65536-16MHz/256/100Hz
	//timer1_counter = 64286;   // preload timer 65536-16MHz/256/50Hz
	//timer1_counter = 34286;   // preload timer 65536-16MHz/256/2Hz

	TCNT1 = timer1_counter;   // preload timer
	TCCR1B |= (1 << CS12);    // 256 prescaler
	TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
	interrupts();             // enable all interrupts
}

ISR(TIMER1_OVF_vect)        // interrupt service routine
{
  interrupts();             // enable all interrupts
  TCNT1 = timer1_counter;   // preload timer
  serviceList();
}