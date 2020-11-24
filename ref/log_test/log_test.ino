bool errorLog;

void Elog(char inData[]){
    if (errorLog == true) {
    Serial.println(inData);
    Serial.println("error");
    }
    }

void setup() {
  Serial.begin(115200);
  Serial.println("serial");
  errorLog = true;
  Elog("this is an error message");
}

void loop() {
  // put your main code here, to run repeatedly:

}
