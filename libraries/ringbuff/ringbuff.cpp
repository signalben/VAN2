#include "Arduino.h"
#include "ringbuff.h"
//Note: message.valid = false indicates empty slot 

//constructs FIFO buffer to store messages in order
ringbuff::ringbuff(void) {
	readFrom = 0; //is the 'start' of ringbuffer i.e keeps track which is the oldest element/slot 
	Available = false; //indicates at least one message in buffer
	Overload = false; //indicates capacity reached, messages have been lost
	msgCnt = 0; //indicates number of messages in buffer
}

//stores valid messages- is not intended for storing invalid messages
void ringbuff::Store(Message inData) {
	Available = true;	//either inData gets stored or buffer is full
	for (int i = 0; i < 10; i++) { //For every message slot in storage
		if (storage[i].valid == false) { //Check not already filled with valid message
			Overload = false;		//if empty
			storage[i] = inData;
			msgCnt++;
			return;
		}
	}
	Elog(FNUM, 1); //ERROR1 failed to store message, ringBuffer over capacity;
	Overload = true;
}

//Returns oldest valid message, clears returned message from buffer  
//If no valid message, returns an invalid one to indicate null
Message ringbuff::Read(void) {
	for (int i = readFrom; i < 10; i++) { //For every message slot in storage
		if (storage[i].valid) { //if valid message in storage slot
			Message outData = storage[i]; //Retreive message
			storage[i].valid = false; //then empty that slot
			msgCnt--;
			readFrom = i + 1; //Set advance slot for next call
			return outData;
		}
	}
	//Buffer is empty upwards of readFrom element, newer messages may have been stored before this element
	//Read from element zero again
	readFrom = 0;
	for (int i = readFrom; i < 10; i++) {
		if (storage[i].valid) { //if valid message in storage slot
			Message outData = storage[i]; //Retreive message
			storage[i].valid = false; //then empty that slot
			msgCnt--;
			readFrom = i + 1; //Set advance slot for next call
			return outData;
		}
	}
	//Buffer is empty
	Available = false;
	Overload = false;
	return storage[0]; //in this case no valid messages, and storage[0].valid = false
}

//prints line indicating state of the buffer to Serial0- used for debugging
void ringbuff::Show(void) {
	for (int i = 0; i < 10; i++) {
		Serial.print(storage[i].valid);
		Serial.print(":");
		Serial.print(storage[i].src);
		Serial.print("_");
	}
	Serial.println();
}
