

#include "Arduino.h"
#include "ringbuff.h"

ringbuff::ringbuff(void) {

	readFrom = 0;   
	Available = false;
	Overload = false;
	msgCnt = 0;
}

void ringbuff::Store(Message inData) {
	Available = true;
	for (int i = 0; i < 10; i++) { //For every message slot in storage
		if (storage[i].valid == false) { //Check not already filled with valid message
			Overload = false;
			storage[i] = inData;
			msgCnt++;
			return;
		}
	}
	//Serial.println("ringBuffer over capacity");
	Overload = true;
	//return;
}

Message ringbuff::Read(void) {
	for (int i = readFrom; i < 10; i++) {
		if (storage[i].valid) { //if valid message in storage slot
			Message outData = storage[i]; //Retreive message
			storage[i].valid = false; //then empty that slot
			msgCnt--;
			readFrom = i + 1; //Set advance slot for next call
			return outData;
		}
	}
	readFrom = 0;
	//Serial.println("Note: reading from slot zero again");
	for (int i = readFrom; i < 10; i++) {
		if (storage[i].valid) { //if valid message in storage slot
			Message outData = storage[i]; //Retreive message
			storage[i].valid = false; //then empty that slot
			msgCnt--;
			readFrom = i + 1; //Set advance slot for next call
			return outData;
		}
	}
	Available = false;
	Overload = false;
	//Serial.println("Nothing in buffer!");
	return storage[0]; //in this case no valid messages, and storage[0].valid = false
}

void ringbuff::Show(void) {
	for (int i = 0; i < 10; i++) {
		Serial.print(storage[i].valid);
		Serial.print(":");
		Serial.print(storage[i].src);
		Serial.print("_");
	}
	Serial.println();
}
