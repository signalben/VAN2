
void setup() {
  Serial.begin(115200);
}

void loop() {
  int velocity = analogRead(36);
  int heading = analogRead(39);
  velocity = (velocity >> 4);
  heading  = (heading  >> 4);
  

  Serial.print(velocity);
  Serial.print("____");
  Serial.println(heading);
  delay(20);
}
