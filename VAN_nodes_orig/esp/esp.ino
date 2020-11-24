
//https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/
//https://www.hobbytronics.co.uk/arduino-timer-interrupts
//https://maker.pro/arduino/projects/timer-interrupts-improve-your-arduino-programming-skills
//http://www.avrbeginners.net/architecture/timers/timers.html
//http://www.8bit-era.cz/arduino-timer-interrupts-calculator.html

#include "netDef.h"
//#include <WiFi.h>
#include <VAN3.h>
//code from: https://techtutorialsx.com/2017/11/13/esp32-arduino-setting-a-socket-server/
//https://techtutorialsx.com/2017/10/07/esp32-arduino-timer-interrupts/
//#include "C:\Program Files (x86)\Arduino\libraries\WiFi\src\WiFi.h"



void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  portEXIT_CRITICAL_ISR(&timerMux);
  serviceList();
}

void commandList() {
}

void serviceList() {
  yield();
  getMessage(2);
  getMessage(3);
  activeSerial = 3;
  //activeSerialWrite(uint8_t('A'));
  //Serial.println("Loop");
}

void setup()
{
  Serial.begin(57600);
  Serial2.begin(57600);
  Serial.print("Board type: ");
  Serial.println(board_type);
  wifiSetup();
  Serial.println("wifi OK");
 // setTimer();
 // Serial.println("Timer OK");
 activeSerial = 3;
 activeSerialWrite(uint8_t('Q'));
 activeSerialWrite(uint8_t('u'));
 activeSerialWrite(uint8_t('l'));
 activeSerialWrite(uint8_t('o'));
}

void loop()
{
  delay(1000);
  serviceList();
}
