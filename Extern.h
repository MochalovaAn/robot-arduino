#ifndef EXTERN_H
#define EXTERN_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

#include "Gyro.h"
#include "Stepper.h"
#include "Program.h"

extern DNSServer dnsServer;
extern AsyncWebServer webServer;
extern AsyncEventSource events;
extern Stepper stepper;
extern Gyro gyro;
extern Program program;

#endif // EXTERN_H