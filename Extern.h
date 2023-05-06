#ifndef EXTERN_H
#define EXTERN_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "ESP8266EventSource.h"
#include <ArduinoJson.h>
#include <LittleFS.h>

#include "Gyro.h"
#include "Stepper.h"
#include "Program.h"

extern DNSServer dnsServer;
extern ESP8266WebServer webServer;
extern ESP8266EventSource events;
extern Stepper stepper;
extern Gyro gyro;
extern Program program;

#endif // EXTERN_H