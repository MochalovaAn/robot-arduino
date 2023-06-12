#ifndef WEB_H
#define WEB_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

bool handleCaptivePortal(AsyncWebServerRequest *request);

void handleStepper(AsyncWebServerRequest *request);
void handleStepperRotate(AsyncWebServerRequest *request);
void handleStepperStop(AsyncWebServerRequest *request);
void handleStepperReset(AsyncWebServerRequest *request);
void handleStepperEnable(AsyncWebServerRequest *request);
void handleStepperDisable(AsyncWebServerRequest *request);

void handleGyro(AsyncWebServerRequest *request);

void handleProgram(AsyncWebServerRequest *request);
void handleProgramText(AsyncWebServerRequest *request);
void handleProgramRun(AsyncWebServerRequest *request);
void handleProgramStop(AsyncWebServerRequest *request);

void handleFile(AsyncWebServerRequest *request);
void handleNotFound(AsyncWebServerRequest *request);

void eventsOnConnect(AsyncEventSourceClient *client);
void eventsSendStatus(AsyncEventSource *events, unsigned long now);

void logRequest(AsyncWebServerRequest *request, bool eol);
void badRequest(AsyncWebServerRequest *request);
void notFound(AsyncWebServerRequest *request);
void methodNotAllowed(AsyncWebServerRequest *request, String allow);
void internalServerError(AsyncWebServerRequest *request);

#endif // WEB_H
