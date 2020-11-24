//https://www.hobbytronics.co.uk/arduino-timer-interrupts

#ifndef VAN3_h
#define VAN3_h
#include "Arduino.h"
//#include "C:\Users\Ben\AppData\Local\Arduino15\packages\esp32\hardware\esp32\1.0.4\libraries\WiFi\src\WiFi.h"
#include <WiFi.h>

    extern uint8_t lobject[];
    extern uint8_t port0[];
    extern uint8_t port1[];
    extern int n_lobject;
    extern int n_port0;
    extern int n_port1;
    extern int board_type;
    extern int activeSerial;

    extern uint8_t message[5];
    extern bool messageValid;
    extern uint8_t  START;
    extern uint8_t  END;
    extern int timer1_counter;

    extern hw_timer_t* timer;
    extern portMUX_TYPE timerMux;

    void IRAM_ATTR onTimer();

    void wifiSetup();

    void ultrasound_command();

    void ultrasound_publish();
    
    void commandList();

    void serviceList();

    void activeSerialWrite(uint8_t outbyte);

    int activeSerialAvailable();

    uint8_t activeSerialRead();

    bool getByte();

    bool getStart();

    void getMessage(int selected);

    void handleMessage();

    void debugMessage();

    void passMessage();

    void setTimer();

#endif