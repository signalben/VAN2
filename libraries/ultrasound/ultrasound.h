//https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/
#ifndef ultrasound_h
#define ultrasound_h
#include "Arduino.h"

extern int trigPin;
extern int echoPin;

extern uint8_t ultrasound; //Defines ID for this object
extern uint8_t PC;		  //Defines ID for default message destination


extern uint8_t message[5]; //Gives access to global message buffer
extern bool messageValid;  //Allows flagging of active/inactive messages
extern uint16_t mask;      //Mask used for converting integers to bytes for transmission

//This objects functions, command called by messages, publish called by timer interupt
void ultrasound_CMD();
void ultrasound_PUB();

//External functions this object needs to call
void handleMessage();
void debugMessage();

#endif