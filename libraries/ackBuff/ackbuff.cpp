#include "Arduino.h"
#include "ackbuff.h"
//notes: 
//message.valid = false indicates empty slot 
//messages should be input as STD, they get changed to ACK only when messages are output

//constructs buffer to store messages- order not retained
ackbuff::ackbuff(void) {
	Available = false; //indicates at least one message in buffer
	Overload = false; //indicates capacity reached, messages have been lost
	msgCnt = 0; //indicates number of messages in buffer
}

//evaluates equality of two messages- every variable must be identical
bool ackbuff::messagesEqual(message a, message b) {
	if (a.valid != b.valid) {
		return false;
	}
	if ((a.start != b.start) || (a.src != b.src) || (a.dest != b.dest) || (a.cmd != b.cmd) || (a.dat0 != b.dat0) || (a.dat1 != b.dat1)) {
		return false;
	}
	return true;
}

//evaluates equality of two messages- all variables, other than the data content must be identical
bool ackbuff::sameFormat(message a, message b) {
	if (a.valid != b.valid) {
		return false;
	}
	if ((a.start != b.start) || (a.src != b.src) || (a.dest != b.dest) || (a.cmd != b.cmd)) {
		return false;
	}
	return true;
}



//returns bool indicating if ackbuff contains the exact message inData 
bool ackbuff::contains(message inData) {
	for (int i = 0; i < 10; i++) {

		if (messagesEqual(inData, storage[i])) { //if same message is awaiting acknowledgment in storage slot
			return true;
		}
	}
	return false;
}

//adds message to buffer, if there's space 
void ackbuff::add(Message inData) {

	if (contains(inData)) { //If already in buffer, do not store duplicate message
		return;
	}

	for (int i = 0; i < 10; i++) { //For every message slot in storage
		if (storage[i].valid == false) { //Check not already filled with valid message
			Overload = false;
			storage[i] = inData;
			msgCnt++;
			Available = true;
			return;
		}
	}
	Elog(FNUM, 1);//ERROR1 failed to store message, ackbuff over capacity
	Overload = true;
}

//clears a message from the buffer if its format is identical to inData, disregards data values
//allows a device to "undo" a request for a response
void ackbuff::cancel(Message inData) {
	for (int i = 0; i < 10; i++) {
		if (sameFormat(inData, storage[i])) { //if same message is awaiting acknowledgment in storage slot
			storage[i].valid = false; //then empty that slot
			msgCnt--;
			return;
		}
	}
	return;
}

//clears a message from the buffer if it is identical to inData, including data values
//allows VAN to remove messages from the buffer only if it can exactly reproduce that message from a recieved response
void ackbuff::clear(Message inData) {
	for (int i = 0; i < 10; i++) {
		if (messagesEqual(inData, storage[i])) { //if same message is awaiting acknowledgment in storage slot
			storage[i].valid = false; //then empty that slot
			msgCnt--;
			return;
		}
	}
	return;
}

//used for repeatedly sending messages in buffer waiting for a response 
//should be added to reportList (main)
void ackbuff::handleWaiting() {

	if (period == 0) {		//disables sending altogether
		return;
	}

	unsigned long now = millis();			//period of sends can be set like VAN devices
	if ((now - lastReport) < period) {
		return;
	}

	for (int i = 0; i < 10; i++) {
		if (storage[i].valid) {
			message buff = storage[i];
			buff.start = ACK;			//messages are stored as STD, but must be flagged as ACK when sent (otherwise no RESP will be provoked on other end)
			showMessage(buff);
			handleMessage(buff);
		}
	}

	lastReport = millis();
}

//resets buffer
void ackbuff::clearAll() {
	for (int i = 0; i < 10; i++) {
		storage[i].valid = false;
	}
	Available = false;
	Overload = false; 
	msgCnt = 0; 
}

//prints line indicating state of the buffer to Serial0- used for debugging
void ackbuff::show(void) {
	for (int i = 0; i < 10; i++) {
		Serial.print(storage[i].valid);
		Serial.print(":");
		Serial.print(storage[i].src);
		Serial.print("_");
	}
	Serial.println();
}
