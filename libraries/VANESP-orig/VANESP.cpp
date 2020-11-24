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





