//code from: https://techtutorialsx.com/2017/11/13/esp32-arduino-setting-a-socket-server/

#include <WiFi.h>
 
const char* ssid = "Rowe Cottage";
const char* password =  "SHIP-rid-exact";
 
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
 
  if (client) {
 
    while (client.connected()) {
 
      while (client.available()>0) {
        char c = client.read();
        client.write(c);
        Serial.println(c);
      }
 
      delay(10);
    }
 
    client.stop();
    Serial.println("Client disconnected");
 
  }
}

/*
import socket               
 
sock = socket.socket()
 
host = "192.168.1.78" #ESP32 IP in local network
port = 80             #ESP32 Server Port    
 
sock.connect((host, port))
 
message = "Hello World"
message = message.encode()#Required for Python 3
sock.send(message)
 
data = ""       
 
while len(data) < len(message):
    data += sock.recv(1)
 
print(data)
 
sock.close()
 */
