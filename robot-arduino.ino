#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include "Stop.h"
#include "Gyro.h"
#include "Stepper.h"
#include "Program.h"
#include "Web.h"

// Wi-Fi params

const char *WIFI_SSID = "Robot";
const char *WIFI_PASSWORD = "";

IPAddress WIFI_LOCAL_IP(172, 16, 0, 1);
IPAddress WIFI_GATEWAY(172, 16, 0, 1);
IPAddress WIFI_NETMASK(255, 255, 0, 0);

// Events send period

#define EVENTS_PERIOD 100

DNSServer DnsServer;
AsyncWebServer WebServer(80);
AsyncEventSource Events("/events");

void setup()
{
  FSInfo fsinfo;

  Serial.begin(115200);
  Serial.println();

  // mount file system

  Serial.println("Mounting file system...");

  if (!LittleFS.begin() || !LittleFS.info(fsinfo))
  {
    Serial.println("Error: Failed to mount files system");
    stop(1);
  }

  Serial.print("FS Total: ");
  Serial.println(fsinfo.totalBytes);
  Serial.print("FS Used: ");
  Serial.println(fsinfo.usedBytes);

  // setup Gyroscope

  // Serial.println("Initializing Gyroscope...");

  // if (!Gyro.init())
  // {
  //   Serial.println("Error: Failed to initalize Gyroscope");
  //   // stop(2);
  // }
  // else
  //   Serial.println("Gyroscope initialized successfully");

  // setup Wi-Fi access point

  Serial.println("Starting Wi-Fi access point...");

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(WIFI_LOCAL_IP, WIFI_GATEWAY, WIFI_NETMASK);
  if (!WiFi.softAP(WIFI_SSID, WIFI_PASSWORD))
  {
    Serial.println("Error: Failed to start Wi-Fi access point");
    stop(3);
  }

  Serial.print("Wi-Fi SSID: ");
  Serial.println(WIFI_SSID);
  Serial.print("Wi-Fi Password: ");
  Serial.println(WIFI_PASSWORD);
  Serial.println("Wi-Fi started successfully");

  // setup the DNS server redirecting all the domains to the Wi-Fi access point

  Serial.println("Starting DNS server...");

  DnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  if (!DnsServer.start(53, "*", WIFI_LOCAL_IP))
  {
    Serial.println("Error: Failed to start DNS server");
    stop(4);
  }

  Serial.print("DNS-server IPv4: ");
  Serial.println(WIFI_LOCAL_IP);
  Serial.println("DNS-server started successfully");

  // setup WEB server page handlers

  Serial.println("Starting WEB server...");

  WebServer.on("/stepper/rotate", handleStepperRotate);
  WebServer.on("/stepper/stop", handleStepperStop);
  WebServer.on("/stepper/reset", handleStepperReset);
  WebServer.on("/stepper/enable", handleStepperEnable);
  WebServer.on("/stepper/disable", handleStepperDisable);
  WebServer.on("/stepper", handleStepper);
  WebServer.on("/gyro", handleGyro);
  WebServer.on("/program/text", handleProgramText);
  WebServer.on("/program/run", handleProgramRun);
  WebServer.on("/program/stop", handleProgramStop);
  WebServer.on("/program", handleProgram);

  WebServer.onNotFound(handleFile);

  Events.onConnect(eventsOnConnect);
  WebServer.addHandler(&Events);

  WebServer.begin();

  Serial.print("Homepage URL: http://");
  Serial.println(WIFI_LOCAL_IP);
  Serial.println("EventSource URL: /events");
  Serial.println("WEB-server started successfully");

  Serial.println("Robot started");
  Serial.println("Waiting connections...");
}

unsigned long timer = 0;

void loop()
{
  DnsServer.processNextRequest();
  Program.execute();
  Stepper.run();

  unsigned long now = millis();
  if (now > timer + EVENTS_PERIOD)
  {
    // Gyro.getMotion();
    eventsSendStatus(&Events, now);
    timer = now;
  }
}
