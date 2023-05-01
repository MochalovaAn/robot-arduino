#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <AccelStepper.h>

#include "ESP8266EventSource.h"
#include "Imu.h"
#include "Program.h"

// Wi-Fi params

const char *WIFI_SSID = "Robot";
const char *WIFI_PASSWORD = "1234567890";

IPAddress WIFI_LOCAL_IP(192, 168, 1, 1);
IPAddress WIFI_GATEWAY(192, 168, 1, 1);
IPAddress WIFI_NETMASK(255, 255, 255, 0);

// ULN2003 motor driver pins

#define STEPPER_IN1 0  // D3
#define STEPPER_IN2 2  // D4
#define STEPPER_IN3 14 // D5
#define STEPPER_IN4 12 // D6

DNSServer dnsServer;
ESP8266WebServer webServer(80);
ESP8266EventSource events("");
AccelStepper stepper(AccelStepper::HALF4WIRE, STEPPER_IN1, STEPPER_IN3, STEPPER_IN2, STEPPER_IN4);
Imu imu;
Program program(&stepper);

void setup()
{
  Serial.begin(115200);
  Serial.println();

  // setup MPU6050

  Serial.println("Starting MPU-6050 IMU...");

  if (!imu.init(20))
  {
    Serial.println("Error: MPU-6050 not found");
    // stop(1);
  }

  Serial.print("Device ID: ");
  Serial.println(imu.getDeviceID());

  // setup Wi-Fi access point

  Serial.println("Starting Wi-Fi access point...");

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(WIFI_LOCAL_IP, WIFI_GATEWAY, WIFI_NETMASK);
  if (!WiFi.softAP(WIFI_SSID, WIFI_PASSWORD))
  {
    Serial.println("Error: Failed to start Wi-Fi access point");
    // stop(2);
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
    // stop(3);
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

  webServer.on("/", handleRoot);
  webServer.on("/events", handleEvents);
  webServer.on("/stepper/acceleration", handleStepperAcceleration);
  webServer.on("/stepper/current-position", handleStepperCurrentPosition);
  webServer.on("/stepper/current-speed", handleStepperCurrentSpeed);
  webServer.on("/stepper/disable", handleStepperDisable);
  webServer.on("/stepper/distance-to-go", handleStepperDistanceToGo);
  webServer.on("/stepper/enable", handleStepperEnable);
  webServer.on("/stepper/is-running", handleStepperIsRunning);
  webServer.on("/stepper/move", handleStepperMove);
  webServer.on("/stepper/reset", handleStepperReset);
  webServer.on("/stepper/speed", handleStepperSpeed);
  webServer.on("/stepper/steps-per-revolution", handleStepperStepsPerRevolution);
  webServer.on("/stepper/stop", handleStepperStop);
  webServer.on("/stepper/target-position", handleStepperTargetPosition);
  webServer.on("/imu/roll", handleImuRoll);
  webServer.on("/imu/pitch", handleImuPitch);
  webServer.on("/imu/yaw", handleImuYaw);
  webServer.on("/imu/t", handleImuTemperature);
  webServer.on("/program/text", handleProgramText);
  webServer.on("/program/start", handleProgramStart);
  webServer.on("/program/stop", handleProgramStop);
  webServer.on("/program/state", handleProgramState);
  webServer.on("/program/line", handleProgramLine);
  webServer.on("/program/is-running", handleProgramIsRunning);
  webServer.onNotFound(handleNotFound);
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
  program.run();
  stepper.run();
  imu.getMotion();

  unsigned long now = millis();
  if (now > timer + 100)
  {
    events.send(String(now).c_str(), "test", now);
    timer = now;
  }
}
