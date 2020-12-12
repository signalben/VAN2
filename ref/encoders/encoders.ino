//REF: http://www.4tronix.co.uk/arduino/sketches/initio_03.ino

int rTrigPin = 2, rDirPin = 4, lTrigPin = 3, lDirPin = 5;
int lVel = 0, rVel = 0; // variables keeping Velof the pulses on each side
unsigned long previousUpdate = 0;
int lVelE, rVelE, lPosE, rPosE, lAccE, rAccE, lOldVelE, rOldVelE;
uint8_t lTarget = 1;
uint8_t rTarget = 30;
int lOut;
int rOut;

uint8_t lP = 20, lI = 1, lD = 8;
uint8_t rP = 20, rI = 1, rD = 8;

void intLeft() { // This interrupt routine runs if the left side encoder is triggered

  if (digitalRead(lTrigPin)) {
    if (digitalRead(lDirPin)) // use DL1 to determine which direction we're moving. HIGH = Forwards
      lVel++;
    else
      lVel--;
  }

  else {
    if (digitalRead(lDirPin)) // use DL1 to determine which direction we're moving. HIGH = Forwards
      lVel--;
    else
      lVel++;
  }
}

void intRight() { // This interrupt routine runs if the left side encoder is triggered (LEC)
  if (digitalRead(rTrigPin)) {
    if (digitalRead(rDirPin)) // use DL1 to determine which direction we're moving. HIGH = Forwards
      rVel--;
    else
      rVel++;
  }

  else {
    if (digitalRead(rDirPin)) // use DL1 to determine which direction we're moving. HIGH = Forwards
      rVel++;
    else
      rVel--;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Reset");

  pinMode(lTrigPin, INPUT);
  pinMode(lDirPin, INPUT);
  pinMode(rTrigPin, INPUT);
  pinMode(rDirPin, INPUT);

  attachInterrupt(digitalPinToInterrupt(lTrigPin), intLeft, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rTrigPin), intRight, CHANGE);
}

void updatePid() {
  lVelE = lTarget - lVel ;
  lPosE += lVelE;
  lAccE = lVelE - lOldVelE;
  lOut = ((lP * lVelE) + (lI * lPosE) + (lD * lAccE))/16;
  lOldVelE = lVelE;

  rVelE = rTarget - rVel ;
  rPosE += rVelE;
  rAccE = rVelE - rOldVelE;
  rOut = ((rP * rVelE) + (rI * rPosE) + (rD * rAccE))/16;
  Serial.println(rOut);
  rOldVelE = rVelE;
}
// the loop routine runs over and over again forever:
void loop()
{
  unsigned long now = millis();
  while (now - previousUpdate < 50) {
    now = millis();
  }

  updatePid();
  previousUpdate = millis();
  lVel = 0;
  rVel = 0;
}
