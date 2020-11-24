


void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  
}

void loop() {
  if(Serial1.available()){
    char data = Serial1.read();
  Serial1.println(data);
  }
}
