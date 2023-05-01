#ifndef WEB_H
#define WEB_H

#include "Extern.h"

bool handleCaptivePortal();

void handleRoot();

void handleStepper();
void handleStepperRotate();
void handleStepperStop();
void handleStepperReset();

void handleGyro();

void handleProgram();
void handleProgramText();
void handleProgramRun();
void handleProgramStop();

void handleEvents();
void eventsOnConnect(ESP8266EventSourceClient *client);
void eventsOnDisconnect(ESP8266EventSourceClient *client);
void eventsSendStatus(unsigned long now);

void handleNotFound();

void logRequest(bool eol);
void badRequest();
void methodNotAllowed(String allow);

#endif // WEB_H
