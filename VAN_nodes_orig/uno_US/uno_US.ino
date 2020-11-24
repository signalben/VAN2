
//https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/
//https://www.hobbytronics.co.uk/arduino-timer-interrupts
//https://maker.pro/arduino/projects/timer-interrupts-improve-your-arduino-programming-skills
//http://www.avrbeginners.net/architecture/timers/timers.html
//http://www.8bit-era.cz/arduino-timer-interrupts-calculator.html

#include "netDef.h"
#include <VAN3.h>
#include <ultrasound.h>

void commandList() {
    if (message[1] == 0x10) {
      ultrasound_CMD();
    }
}

void serviceList() {
    getMessage(0);
  ultrasound_PUB();
}

ISR(TIMER1_OVF_vect)        // interrupt service routine
{
    //interrupts();             // enable all interrupts
    TCNT1 = timer1_counter;   // preload timer
    serviceList();
}

void setup()
{
  Serial.begin(57600);
  Serial.println("speaking");
  setTimer();
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT); 
}


void loop()
{
  delay(100000);
}
