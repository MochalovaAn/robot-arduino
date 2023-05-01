#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "ESP8266EventSource.h"
#include <AccelStepper.h>
#include <LittleFS.h>

#include "Stop.h"
#include "Gyro.h"
#include "Program.h"
#include "Web.h"

// Wi-Fi params

const char *WIFI_SSID = "Robot";
const char *WIFI_PASSWORD = "";

IPAddress WIFI_LOCAL_IP(172, 16, 0, 1);
IPAddress WIFI_GATEWAY(172, 16, 0, 1);
IPAddress WIFI_NETMASK(255, 255, 0, 0);

// ULN2003 motor driver pins

#define STEPPER_IN1 0  // D3
#define STEPPER_IN2 2  // D4
#define STEPPER_IN3 14 // D5
#define STEPPER_IN4 12 // D6

// Gyro update period

#define GYRO_PERIOD 100 

DNSServer dnsServer;
ESP8266WebServer webServer(80);
ESP8266EventSource events("");
AccelStepper stepper(AccelStepper::HALF4WIRE, STEPPER_IN1, STEPPER_IN3, STEPPER_IN2, STEPPER_IN4);
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

  Serial.println("Starting MPU-6050...");

  if (!gyro.init(GYRO_PERIOD))
  {
    Serial.println("Error: MPU-6050 not found");
    // stop(2);
  }

  Serial.print("Device ID: ");
  Serial.println(gyro.getDeviceID());

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

  events.onConnect(eventsOnConnect);
  events.onDisconnect(eventsOnDisconnect);
  events.begin(&webServer);

  webServer.on("/stepper", handleStepper);
  webServer.on("/stepper/rotate", handleStepperRotate);
  webServer.on("/stepper/stop", handleStepperStop);
  webServer.on("/stepper/reset", handleStepperReset);
  webServer.on("/stepper/enable", handleStepperEnable);
  webServer.on("/stepper/disable", handleStepperDisable);
  webServer.on("/gyro", handleGyro);
  webServer.on("/program", handleProgram);
  webServer.on("/program/text", handleProgramText);
  webServer.on("/program/run", handleProgramRun);
  webServer.on("/program/stop", handleProgramStop);
  webServer.on("/events", handleEvents);

  webServer.onNotFound(handleFile);

  webServer.collectHeaders("Last-Event-ID", "Content-Type");
  webServer.begin();

  Serial.println("Robot started");
  Serial.println("Waiting connections...");
}

unsigned long timer = 0;

void loop()
{
  dnsServer.processNextRequest();
  webServer.handleClient();
  events.keepAlive();
  program.execute();
  stepper.run();
  gyro.getMotion();

  unsigned long now = millis();
  if (now > timer + GYRO_PERIOD)
  {
    eventsSendStatus(now);
    timer = now;
  }
}
