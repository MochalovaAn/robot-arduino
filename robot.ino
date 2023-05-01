#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AccelStepper.h>

#include "Imu.h"

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
AsyncWebServer webServer(80);
AsyncEventSource sseServer("/events");
AccelStepper stepper(AccelStepper::HALF4WIRE, STEPPER_IN1, STEPPER_IN3, STEPPER_IN2, STEPPER_IN4);
Imu imu;

void setup()
{
  Serial.begin(115200);
  Serial.println();

  // setup MPU6050

  Serial.println("Starting MPU-6050 IMU...");

  if (!imu.init(20))
  {
    Serial.println("Error: MPU-6050 IMU not found");
    Serial.println();
    stop(1);
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
    stop(2);
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
    stop(3);
  }

  Serial.print("DNS-server IPv4: ");
  Serial.println(WIFI_LOCAL_IP);

  // setup WEB server page handlers
  Serial.println("Starting WEB server...");

  Serial.print("WEB-server URL: http://");
  Serial.println(WIFI_LOCAL_IP);
  Serial.println("EventSource URL: /events");

  sseServer.onConnect(handleOnConnect);

  webServer.on("/", handleCaptivePortal);
  webServer.on("/stepper/acceleration", handleStepperAcceleration);
  webServer.on("/stepper/currentPosition", handleStepperCurrentPosition);
  webServer.on("/stepper/currentSpeed", handleStepperCurrentSpeed);
  webServer.on("/stepper/disable", handleStepperDisable);
  webServer.on("/stepper/distanceToGo", handleStepperDistanceToGo);
  webServer.on("/stepper/enable", handleStepperEnable);
  webServer.on("/stepper/isRunning", handleStepperIsRunning);
  webServer.on("/stepper/move", handleStepperMove);
  webServer.on("/stepper/moveTo", handleStepperMoveTo);
  webServer.on("/stepper/reset", handleStepperReset);
  webServer.on("/stepper/speed", handleStepperSpeed);
  webServer.on("/stepper/stepsPerRevolution", handleStepperStepsPerRevolution);
  webServer.on("/stepper/stop", handleStepperStop);
  webServer.on("/stepper/targetPosition", handleStepperTargetPosition);
  webServer.on("/imu/roll", handleImuRoll);
  webServer.on("/imu/pitch", handleImuPitch);
  webServer.on("/imu/yaw", handleImuYaw);
  webServer.on("/imu/t", handleImuTemperature);
  webServer.onNotFound(handleNotFound);
  webServer.addHandler(&sseServer);
  webServer.begin();

  Serial.println("Robot started");
  Serial.println("Waiting connections...");
}

unsigned long timer = 0;

void loop()
{
  dnsServer.processNextRequest();
  stepper.run();
  imu.getMotion();

  // if (imu.getMotion())
  // {
  //   Serial.print(imu.roll);
  //   Serial.print(" ");
  //   Serial.print(imu.pitch);
  //   Serial.print(" ");
  //   Serial.print(imu.yaw);
  //   Serial.println();
  // }

  unsigned long now = millis();
  if (now - timer > 100)
  {
    timer = now;

    if (sseServer.count())
    {
      Serial.printf("%u %u\n", sseServer.count(), sseServer.avgPacketsWaiting());

      if (!sseServer.avgPacketsWaiting())
      {
        Serial.println("Send event");
        sseServer.send(String(now).c_str(), "test", now);
      }
    }
  }
}
