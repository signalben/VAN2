#ifndef vanMega_h
#define vanMega_h


#include "Arduino.h"
#include <message.h>


extern int vanSerial;
extern uint8_t START;
extern uint8_t END;
extern bool verboseLog;
extern bool errorLog;

extern uint8_t lobject[];
extern uint8_t port0[];
extern uint8_t port1[];
extern uint8_t port2[];
extern uint8_t port3[];

extern uint8_t n_lobject;
extern uint8_t n_port0;
extern uint8_t n_port1;
extern uint8_t n_port2;
extern uint8_t n_port3;


void Vlog(char inData[]);

void Elog(char inData[]);

void vanWrite(uint8_t outbyte);

int vanAvailable();

uint8_t vanRead();

bool getStart();

Message getMessage(int selected);

void handleMessage(Message inData);

void sendMessage(Message inData);

void showMessage(Message inData);

void commandList(Message inData);

//void serviceList(Message inData);

#endif