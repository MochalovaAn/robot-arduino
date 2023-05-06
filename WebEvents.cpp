#include "Web.h"

void handleEvents()
{
  if (handleCaptivePortal())
    return;

  if (webServer.method() != HTTP_GET)
  {
    methodNotAllowed("GET");
    return;
  }

  if (!events.handleRequest())
  {
    Serial.println("503");
    return;
  }
}

void eventsOnConnect(ESP8266EventSourceClient *client)
{
  unsigned long id = millis();

  Serial.println(200); // <- end of request log line

  Serial.print("EventSource: Connected ");
  Serial.print(client->remoteIP());
  Serial.print(", client ");
  Serial.print(client->clientId());
  Serial.print(", last ");
  Serial.println(client->lastId());

  client->send("hello!", NULL, id, 10000);
}

void eventsOnDisconnect(ESP8266EventSourceClient *client)
{
  Serial.print("EventSource: Disconnected ");
  Serial.print(client->remoteIP());
  Serial.print(", client ");
  Serial.print(client->clientId());
  Serial.println();
}

void eventsSendStatus(unsigned long now)
{
    DynamicJsonDocument doc(1024);

    doc["millis"] = now; 
    doc["clients"] = events.count();

    doc["stepper"]["acceleration"] = stepper.acceleration();
    doc["stepper"]["speed"] = stepper.maxSpeed();
    doc["stepper"]["currentSpeed"] = stepper.speed();
    doc["stepper"]["currentPosition"] = stepper.currentPosition();
    doc["stepper"]["targetPosition"] = stepper.targetPosition();
    doc["stepper"]["distanceToGo"] = stepper.distanceToGo();
    doc["stepper"]["isRunning"] = stepper.isRunning();
    doc["stepper"]["stepsPerRevolution"] = stepper.stepsPerRevolution();

    doc["gyro"]["deviceID"] = gyro.getDeviceID();
    doc["gyro"]["roll"] = gyro.roll;
    doc["gyro"]["pitch"] = gyro.pitch;
    doc["gyro"]["yaw"] = gyro.yaw;
    doc["gyro"]["ax"] = gyro.ax;
    doc["gyro"]["ay"] = gyro.ay;
    doc["gyro"]["az"] = gyro.az;
    doc["gyro"]["gx"] = gyro.gx;
    doc["gyro"]["gy"] = gyro.gy;
    doc["gyro"]["gz"] = gyro.gz;
    doc["gyro"]["t"] = gyro.t;

    doc["program"]["state"] = program.state();
    doc["program"]["line"] = program.line();
    doc["program"]["count"] = program.count();
    doc["program"]["cycle"] = program.cycle();
    doc["program"]["isRunning"] = program.isRunning();
    doc["program"]["timer"] = program.timer();
    doc["program"]["pause"] = program.pause();

    String data;
    serializeJson(doc, data);

    events.send(data.c_str(), "status", now);
}
