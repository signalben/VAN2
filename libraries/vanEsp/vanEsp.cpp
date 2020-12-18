//References:
//1.https://techtutorialsx.com/2019/10/20/esp32-getting-started-with-esp-now/
//Ref1 was used to create functions: espNowISR, espNowSetup, and to transmit messages via ESP-NOW in handleMessages
#include "vanEsp.h"
ringbuff RB0; //Buffer for storing messages recieved via ESP-NOW until they are handled in sequence
int vanSerial = 0; //Indicates to multiple functions which serial port is currently active
esp_now_peer_info_t peerInfo; //object required for ESP-NOW

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

//function captures messages recieved via ESP-NOW, stores them for use later
void espNowISR(const uint8_t* mac, const uint8_t* data, int len) {

	if (len != 7) {//ERROR ESP-NOW message wrong length;
		return;
	}

	if ((data[0] != STD) && (data[0] != ACK) && (data[0] != RESP)) {//If first byte is not a valid start
		return;
	}

	//The incoming data has been veryfied as the right length, with a valid start, but also needs a checksum
	Message buff = { data[0], data[1], data[2], data[3], data[4], data[5], false };	//So create an unvalidated message from it
	uint8_t checksum = calcChecksum(buff); //and compute it's checksum by XOR'ing all recieved variables
	if (data[6] != checksum) {	//Compare to received checksum byte
		return;	
	}
	buff.valid = true;
	RB0.Store(buff);	//stores for use later
}

void espNowSetup() {
	WiFi.mode(WIFI_STA);		//put wifi in station mode
	if (esp_now_init() != ESP_OK) {	//initialize ESP-NOW
		Elog(FNUM, 6); //ERROR6 Error initializing ESP-NOW
		return;
	}
	esp_now_register_recv_cb(espNowISR);	//specify function espNowISR to as ISR to call when bytes are recieved
	memcpy(peerInfo.peer_addr, peerMAC, 6);	//copy other ESP32's MAC address, peerMAC, to object peerInfo
	peerInfo.channel = 0;
	peerInfo.encrypt = false;				
	if (esp_now_add_peer(&peerInfo) != ESP_OK) { //add peerInfo (of datatype esp_now_peer_info_t) to ESP-NOW
		Elog(FNUM, 7);  //ERROR7 Failed to add peer
		return;
	}
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

	for (i = 0; i < N_SPORT3; i++) {   //If message destination is via port 3, send via ESP-NOW
		if (SPORT3[i] == inData.dest) {
			uint8_t  checksum = calcChecksum(inData);	//its checksum is required to do this 
			uint8_t buff[7] = { inData.start, inData.src, inData.dest, inData.cmd, inData.dat0, inData.dat1, checksum }; //store message values in array of bytes
			esp_err_t result = esp_now_send(peerMAC, (uint8_t*)&buff, 7); //result, of datatype esp_err_t indicates if array buff[] has been sent to address peerMAC
			if (result != ESP_OK) {
				Elog(FNUM, 5);//ERROR5- Error sending message via ESP-NOW
			}
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

	Elog(FNUM, 4);//ERROR4- Message has destination unknown to this node
}

//returns 1 message from specified serial port
Message getMessage(int selected) {
	vanSerial = selected; //Sets active serial port
	Message buff;		  //Construct empty message
	buff.valid = false;   //Marked as invalid until it is complete

	if (vanSerial != 3) { //ports other than 3 are hardware serial, so message is composed of bytes read from hardware buffer
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
			return buff;	//message is corrupt, cancel
		}
		buff.valid = true; //Allows further use by other functions
	}
	else { //VanSerial 3 reads from a ringbuffer, where ESP-NOW messages are put
		buff = RB0.Read(); //returns oldest message in ringbuffer, (validity already checked before being stored in RB0)
		if (buff.valid == false) { //indicates RB0 has no valid message
			return buff;	//there is no message, cancel
		}
	}

	if (buff.start != STD) { //if the message is ACK or RESP
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

//function prints messages to Serial0 in a more human friendly format- used for debugging
void showMessage(Message inData) {
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

