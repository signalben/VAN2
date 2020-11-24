//https://techtutorialsx.com/2017/11/13/esp32-arduino-setting-a-socket-server/

#include <WiFi.h>

const char* ssid = "TNCAP6C5D86";
const char* password =  "F98D5947AD";

WiFiServer wifiServer(80);

void setup() {

  Serial.begin(115200);

  delay(1000);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());

  wifiServer.begin();
}

void loop() {

  WiFiClient client = wifiServer.available();
  Serial.println("Client=");

  if (client) {
    Serial.println("Client true"); //true when if PC connects

    while (client.connected()) {
   //   Serial.println("Client connected"); //if PC connects

      while (client.available() > 0) {
        Serial.print("Client available: ");
        Serial.println(client.available());
        char c = client.read();
        Serial.print("Read: ");
        Serial.println(c);
        Serial.print("Writing: ");
        Serial.println(c);
        client.write(c);
      }

      delay(10);
    }

    client.stop();
    Serial.println("Client disconnected");

  }
}
