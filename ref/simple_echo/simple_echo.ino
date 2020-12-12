void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial.print("running...");
}

void loop() {
  uint8_t inByte;
  if (Serial1.available() == true) {
    inByte = Serial1.read();
    Serial.write(inByte);
  }
Serial1.print("message from MEGA");
}
