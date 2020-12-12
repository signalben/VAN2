
int 
aDir = 12,
aPwm = 3,
bDir = 13,
bPwm = 11;




void setup() {
  pinMode(aDir, OUTPUT);
  pinMode(aPwm, OUTPUT);
  pinMode(bDir, OUTPUT);
  pinMode(bPwm, OUTPUT);

}

void loop() {
  digitalWrite(aDir, HIGH);
  analogWrite(aPwm, 70);
  digitalWrite(bDir, HIGH);
  analogWrite(bPwm, 70);

  delay(3000);

  digitalWrite(aDir, LOW);
  analogWrite(aPwm, 70);
  digitalWrite(bDir, LOW);
  analogWrite(bPwm, 70);

  delay(3000);

}
