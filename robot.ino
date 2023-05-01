#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <AccelStepper.h>
#include <MPU650.h>

// Wi-Fi params

const char *WIFI_SSID = "Robot";
const char *WIFI_PASSWORD = "1234567890";
IPAddress WIFI_IP(192, 168, 1, 1);
IPAddress WIFI_NETMASK(255, 255, 255, 0);

// Servers params

const byte DNS_PORT = 53;
const byte WEB_PORT = 80;

// ULN2003 motor driver pins

#define STEPPER_IN1 0   // D3
#define STEPPER_IN2 2   // D4
#define STEPPER_IN3 14  // D5
#define STEPPER_IN4 12  // D6

DNSServer dnsServer;
ESP8266WebServer webServer(WEB_PORT);
AccelStepper stepper(AccelStepper::HALF4WIRE, STEPPER_IN1, STEPPER_IN3, STEPPER_IN2, STEPPER_IN4);

void setup()
{
  Serial.begin(115200);
  Serial.println();

  // setup MPU650

  // setup Wi-Fi access point
  Serial.println("Starting Wi-Fi access point...");

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(WIFI_IP, WIFI_IP, WIFI_NETMASK);
  if (!WiFi.softAP(WIFI_SSID, WIFI_PASSWORD))
  {
    Serial.println("Error: Failed to start Wi-Fi access point");
    Serial.println();
  }
  else
  {
    Serial.print("Wi-Fi SSID: ");
    Serial.println(WIFI_SSID);
    Serial.print("Wi-Fi Password: ");
    Serial.println(WIFI_PASSWORD);
  }

  // setup the DNS server redirecting all the domains to the Wi-Fi access point
  Serial.println("Starting DNS server...");

  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  if (!dnsServer.start(DNS_PORT, "*", WIFI_IP))
  {
    Serial.println("Error: Failed to start DNS server");
    Serial.println();
  }
  else
  {
    Serial.print("DNS-server IPv4: ");
    Serial.println(WIFI_IP);
  }

  // setup WEB server page handlers
  Serial.println("Starting WEB server...");

  Serial.print("WEB-server URL: http://");
  Serial.println(WIFI_IP);

  webServer.on("/", handleRoot);
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
  webServer.onNotFound(handleNotFound);
  webServer.begin();

  Serial.println("Robot started");
  Serial.println("Waiting connections...");
}

void loop()
{
  dnsServer.processNextRequest();
  webServer.handleClient();
  stepper.run();
}
