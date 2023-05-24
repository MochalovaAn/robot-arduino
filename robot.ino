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

DNSServer dnsServer;
AsyncWebServer webServer(80);
AsyncEventSource events("/events");
Stepper stepper;
Gyro gyro;
Program program(&stepper);

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

  // setup MPU-6050

  // Serial.println("Starting MPU-6050...");

  // if (!gyro.init(EVENTS_PERIOD))
  // {
  //   Serial.println("Error: MPU-6050 not found");
  //   // stop(2);
  // }

  // Serial.print("Device ID: ");
  // Serial.println(gyro.deviceID);

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

  // setup the DNS server redirecting all the domains to the Wi-Fi access point

  Serial.println("Starting DNS server...");

  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  if (!dnsServer.start(53, "*", WIFI_LOCAL_IP))
  {
    Serial.println("Error: Failed to start DNS server");
    stop(4);
  }

  Serial.print("DNS-server IPv4: ");
  Serial.println(WIFI_LOCAL_IP);

  // setup WEB server page handlers

  Serial.println("Starting WEB server...");

  Serial.print("WEB-server URL: http://");
  Serial.println(WIFI_LOCAL_IP);
  Serial.println("EventSource URL: /events");

  webServer.on("/stepper/rotate", handleStepperRotate);
  webServer.on("/stepper/stop", handleStepperStop);
  webServer.on("/stepper/reset", handleStepperReset);
  webServer.on("/stepper/enable", handleStepperEnable);
  webServer.on("/stepper/disable", handleStepperDisable);
  webServer.on("/stepper", handleStepper);
  webServer.on("/gyro", handleGyro);
  webServer.on("/program/text", handleProgramText);
  webServer.on("/program/run", handleProgramRun);
  webServer.on("/program/stop", handleProgramStop);
  webServer.on("/program", handleProgram);

  webServer.onNotFound(handleFile);

  events.onConnect(eventsOnConnect);
  webServer.addHandler(&events);

  webServer.begin();

  Serial.println("Robot started");
  Serial.println("Waiting connections...");
}

unsigned long timer = 0;

void loop()
{
  dnsServer.processNextRequest();
  program.execute();
  stepper.run();

  // if (gyro.getMotion()) eventsSendStatus(millis());

  unsigned long now = millis();
  if (now > timer + EVENTS_PERIOD)
  {
    eventsSendStatus(now);
    timer = now;
  }
}
