#include "vanMega.h"
int vanSerial = 0; //Indicates to multiple functions which serial port is currently active
int corrupted = 0; //keeps tally of messages which fail checksum

//function passes error codes from other functions to a destination specified in netDef.h
void Elog(uint8_t fileNo, uint8_t errorcode) {
	if (ELOGDEST == 0) { //If destination is 0 elogging is not active
		return;
	}

	//constructs message detailing: the node which encoutered the error(SRC), the file which produced the error(DAT0) and an errorcode (DAT1)
	Message buff;
	buff.set(STD, THISNODE, ELOGDEST, MSGERROR, fileNo, errorcode, true);
	handleMessage(buff); //sends message
}

//function writes single byte(uint8_t) to the active serial port (vanSerial)
void vanWrite(uint8_t outbyte) {
	if (vanSerial == 0) {
		Serial.write(outbyte);
		return;
	}
	if (vanSerial == 1) {
		Serial1.write(outbyte);
		return;
	}
	if (vanSerial == 2) {
		Serial2.write(outbyte);
		return;
	}
	if (vanSerial == 3) {
		Serial3.write(outbyte);
		return;
	}
	Elog(FNUM, 1); //ERROR1 - Write to nonexistant serial port
}

//function returns number of bytes in buffer of active serial port
int vanAvailable() {
	if (vanSerial == 0) {
		return Serial.available();
	}
	if (vanSerial == 1) {
		return Serial1.available();
	}
	if (vanSerial == 2) {
		return Serial2.available();
	}
	if (vanSerial == 3) {
		return Serial3.available();
	}
	Elog(FNUM, 2); //ERROR2- Checking buffer of nonexistant serial port
	return 0;
}

//function returns first byte in buffer of active serial port (also clears byte from buffer)
uint8_t vanRead() {
	if (vanSerial == 0) {
		return Serial.read();
	}
	if (vanSerial == 1) {
		return Serial1.read();
	}
	if (vanSerial == 2) {
		return Serial2.read();
	}
	if (vanSerial == 3) {
		return Serial3.read();
	}
	Elog(FNUM, 3); //ERROR3- Reading nonexistant serial port
	return 0;
}

//function returns a valid start of message, start byte indicates if message is standard, requires acknowledgement, or is an acknowledgement response
uint8_t getStart() {
	while (vanAvailable() >= 7) {//While there are enough bytes for a complete message
		uint8_t inByte = vanRead();

		if ((inByte == STD) || (inByte == ACK) || (inByte == RESP)) {//Message start byte detected
			return inByte;
		}
	}
	return 0; //indicates a message is not yet available
}

//function returns a checksum based on message content, used to detect messages corrupted during transmission
uint8_t calcChecksum(message inData) {
	uint8_t checksum = (((inData.src ^ inData.dest) ^ (inData.cmd ^ inData.dat0)) ^ (inData.start ^ inData.dat1));//XOR all bytes 
	return checksum;
}

//function routes messages based on their destination
void handleMessage(Message inData) {

	if (inData.valid == false) { //Other functions may return junk messages with .valid = false to indicate corrupt or null
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
		if (SPORT0[i] == inData.dest) {	//SPORTx[], from netDef.h, defines correct serial port for given .dest 
			vanSerial = 0;				//Sets active serial port
			sendMessage(inData); //Sends the message
			return;
		}
	}

	for (i = 0; i < N_SPORT1; i++) {   //Check if message destination is via port 1
		if (SPORT1[i] == inData.dest) {
			vanSerial = 1;
			sendMessage(inData);
			return;
		}
	}

	for (i = 0; i < N_SPORT2; i++) {   //Check if message destination is via port 2
		if (SPORT2[i] == inData.dest) {
			vanSerial = 2;
			sendMessage(inData);
			return;
		}
	}

	for (i = 0; i < N_SPORT3; i++) {   //Check if message destination is via port 3
		if (SPORT3[i] == inData.dest) {
			vanSerial = 3;
			sendMessage(inData);
			return;
		}
	}

	Elog(FNUM, 4);//ERROR4- Message has destination unknown to this node
}

//returns 1 message from specified serial port
Message getMessage(int selected) {
	vanSerial = selected; //Sets active serial port
	Message buff;		  //Construct empty message
	buff.valid = false;   //Marked as invalid until it is completed

	uint8_t start = getStart(); //Attempt to get start byte (also determines if message type is STD, ACK, RESP)
	if (start == 0) { //indicates no message start byte found
		return buff; //Return message to ignore, as .valid = false
	}

	buff.start = start;	  //start byte must be kept to retain ACK type of message
	buff.src = vanRead(); //Fill message variables with sequential reads
	buff.dest = vanRead();
	buff.cmd = vanRead();
	buff.dat0 = vanRead();
	buff.dat1 = vanRead();

	uint8_t checksum = calcChecksum(buff); //Compute checksum by XOR'ing all recieved variables
	if (vanRead() != checksum) {	//Compare to received checksum
		corrupted++;
		return buff;	//message is corrupt, take no action
	}
	buff.valid = true; //Allows further use by other functions 

	if (buff.start != STD) { //if the message is ACK or RESP
		//showMessage(buff);
		int i = 0;
		for (i = 0; i < (0 + N_LOCALDEVICE); i++) { //Check if message destination is a local device
			if (LOCALDEVICE[i] == buff.dest) {
				if (buff.start == ACK) {
					Message standardMSG = buff;
					standardMSG.start = STD;//produce copy, flag as normal message 
					buff.sendBack(); //swap source and dest
					buff.start = RESP; //flag as a response
					handleMessage(buff); // send in the same way as an ordinary message
					return standardMSG; //return without ACK qualification (response already delt with)
				}
				if (buff.start == RESP) {
					buff.sendBack(); //swap source and dest
					buff.start = STD; //recover original message (as stored in AKB0) from RESP
					AKB0.clear(buff); //delete it from ackbuff (stop sending it)
					buff.valid = false;// don't return a valid message 
				}
			}
		}
	}

	return buff;// return valid STD message
}

//function writes message to active serial port (vanSerial)
void sendMessage(Message inData) {
	vanWrite(inData.start);
	vanWrite(inData.src);
	vanWrite(inData.dest);
	vanWrite(inData.cmd);
	vanWrite(inData.dat0);
	vanWrite(inData.dat1);
	uint8_t checksum = calcChecksum(inData); //checksum is end byte, will be recomputed and compared at destination node
	vanWrite(checksum);
	return;
}

/*//Function used only for testing handleMessage, shows message, checksum and port it would have been sent on
void sendMessage(Message inData) {
	Serial.println("Message content: ");
	showMessage(inData);
	Serial.println("Message checksum: ");
	uint8_t checksum = calcChecksum(inData); //checksum is end byte, will be recomputed and compared at destination node
	Serial.println(checksum);
	Serial.println("Sent via serial port: ");
	Serial.println(vanSerial);
	Serial.println();
	return;
}*/

//function prints messages to Serial0 in a more human friendly format- used for debugging
void showMessage(Message inData) {
	if (inData.valid == false) {
		return;
	}
	Serial.print(inData.start);
	Serial.print("_");
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

