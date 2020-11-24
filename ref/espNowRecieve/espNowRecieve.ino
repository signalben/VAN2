//https://techtutorialsx.com/2019/10/20/esp32-getting-started-with-esp-now/

#include <esp_now.h>
#include <WiFi.h>


//This device (slave) address  = {0xFC, 0xF5, 0xC4, 0x0F, 0xBE, 0xFC}; Get via Serial.println(WiFi.macAddress());
uint8_t masterAddress[]        = {0x10, 0x52, 0x1C, 0x68, 0xF3, 0xB0};
esp_now_peer_info_t peerInfo;
bool rflag = false;

void onReceiveData(const uint8_t *mac, const uint8_t *data, int len) {
  Serial.print("Message: ");
  for (int i = 0; i < 7; i++) {
    Serial.printf("%02X", data[i]);
    Serial.print(" ");
  }
  Serial.println();
  rflag = true;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Slave alive");
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(onReceiveData);
  memcpy(peerInfo.peer_addr, masterAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  if (rflag == true) {
    esp_send();
    rflag = false;
  }
}

void esp_send() {
  //message: start, source, dest, CMD, DAT, DAT, END
  uint8_t message[7] = {0x07, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00};
  esp_err_t result = esp_now_send(masterAddress, (uint8_t *) &message, 7);
  if (result == ESP_OK) {
    //Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
}
