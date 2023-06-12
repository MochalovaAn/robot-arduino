#include "Web.h"
#include "Stepper.h"
#include "Gyro.h"
#include "Program.h"

void eventsOnConnect(AsyncEventSourceClient *client)
{
  unsigned long id = millis();

  if (client->lastId())
  {
    Serial.print("EventSource - Reconnected ");
    Serial.print(client->client()->remoteIP());
    Serial.print(", last ");
    Serial.println(client->lastId());
  }
  else
  {
    Serial.print("EventSource - Connected ");
    Serial.println(client->client()->remoteIP());
  }

  client->send("hello!", NULL, id, 10000);
}

void eventsSendStatus(AsyncEventSource *events, unsigned long now)
{
  DynamicJsonDocument doc(1024);

  doc["millis"] = now;
  doc["clients"] = events->count();

  doc["stepper"]["acceleration"] = Stepper.acceleration();
  doc["stepper"]["speed"] = Stepper.maxSpeed();
  doc["stepper"]["currentSpeed"] = Stepper.speed();
  doc["stepper"]["currentPosition"] = Stepper.currentPosition();
  doc["stepper"]["targetPosition"] = Stepper.targetPosition();
  doc["stepper"]["distanceToGo"] = Stepper.distanceToGo();
  doc["stepper"]["isRunning"] = Stepper.isRunning();
  doc["stepper"]["stepsPerRevolution"] = STEPS_PER_REVOLUTION;

  doc["gyro"]["deviceID"] = Gyro.deviceID;
  doc["gyro"]["roll"] = Gyro.roll;
  doc["gyro"]["pitch"] = Gyro.pitch;
  doc["gyro"]["yaw"] = Gyro.yaw;

  doc["program"]["state"] = Program.state();
  // doc["program"]["line"] = Program.line();
  // doc["program"]["count"] = Program.count();
  // doc["program"]["cycle"] = Program.cycle();
  doc["program"]["isRunning"] = Program.isRunning();
  // doc["program"]["timer"] = Program.timer();
  // doc["program"]["pause"] = Program.pause();

  String data;
  serializeJson(doc, data);

  events->send(data.c_str(), "status", now);
}
