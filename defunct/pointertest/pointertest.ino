//https://stackoverflow.com/questions/13875425/sending-an-integer-serially-in-arduino

int data = 6543;

uint16_t mask   = B11111111;
uint8_t a = data >> 8;
uint8_t b = data & mask;


void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("READY");
  Serial.println(a);
  Serial.println(b);
  int data2 = (a<<8)|b;
    Serial.println(data2);
}



void loop() {
  
delay(10);
}
