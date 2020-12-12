#ifndef vanMega_h
#define vanMega_h


#include "Arduino.h"
#include <message.h>


extern int vanSerial;
extern const uint8_t START;
extern const uint8_t END;
extern bool verboseLog;
extern bool errorLog;

extern const uint8_t LOCALDEVICE[];
extern const uint8_t SPORT0[];
extern const uint8_t SPORT1[];
extern const uint8_t SPORT2[];
extern const uint8_t SPORT3[];

extern const uint8_t N_LOCALDEVICE;
extern const uint8_t N_SPORT0;
extern const uint8_t N_SPORT1;
extern const uint8_t N_SPORT2;
extern const uint8_t N_SPORT3;


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