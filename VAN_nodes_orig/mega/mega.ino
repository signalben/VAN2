
//https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/
//https://www.hobbytronics.co.uk/arduino-timer-interrupts
//https://maker.pro/arduino/projects/timer-interrupts-improve-your-arduino-programming-skills
//http://www.avrbeginners.net/architecture/timers/timers.html
//http://www.8bit-era.cz/arduino-timer-interrupts-calculator.html

#include "netDef.h"
#include <VAN3.h>

void commandList() {
}

void serviceList() {
    getMessage(0);
    getMessage(1);
    //Serial.println("Serive");
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
  Serial1.begin(57600);
  Serial.print("Multiple ports: ");
  Serial.println(multiPort);
  setTimer();
}


void loop()
{
  delay(100000);
}
