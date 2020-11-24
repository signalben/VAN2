#ifndef vanEsp_h
#define vanEsp_h


#include "Arduino.h"
#include <message.h>
#include <ringbuff.h>
#include <esp_now.h>
#include <WiFi.h>

//extern peerInfo
extern uint8_t peerMAC[];
extern esp_now_peer_info_t peerInfo;
extern int vanSerial;
extern uint8_t START;
extern ringbuff RB0;
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

void espNowISR(const uint8_t* mac, const uint8_t* data, int len);

void espNowSetup();

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