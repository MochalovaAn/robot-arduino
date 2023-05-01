#ifndef WEB_H
#define WEB_H

#include "Extern.h"

bool handleCaptivePortal();

void handleStepper();
void handleStepperRotate();
void handleStepperStop();
void handleStepperReset();
void handleStepperEnable();
void handleStepperDisable();

void handleGyro();

void handleProgram();
void handleProgramText();
void handleProgramRun();
void handleProgramStop();

void handleEvents();
void eventsOnConnect(ESP8266EventSourceClient *client);
void eventsOnDisconnect(ESP8266EventSourceClient *client);
void eventsSendStatus(unsigned long now);

void handleFile();

void logRequest(bool eol);
void badRequest();
void notFound();
void methodNotAllowed(String allow);
void internalServerError();

#endif // WEB_H
