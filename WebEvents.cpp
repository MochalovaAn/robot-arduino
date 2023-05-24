#include "Web.h"

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

  // doc["gyro"]["deviceID"] = gyro.deviceID;
  // doc["gyro"]["roll"] = gyro.roll;
  // doc["gyro"]["pitch"] = gyro.pitch;
  // doc["gyro"]["yaw"] = gyro.yaw;
  // doc["gyro"]["ax"] = gyro.ax;
  // doc["gyro"]["ay"] = gyro.ay;
  // doc["gyro"]["az"] = gyro.az;
  // doc["gyro"]["gx"] = gyro.gx;
  // doc["gyro"]["gy"] = gyro.gy;
  // doc["gyro"]["gz"] = gyro.gz;
  // doc["gyro"]["t"] = gyro.t;

  doc["program"]["state"] = program.state();
  // doc["program"]["line"] = program.line();
  // doc["program"]["count"] = program.count();
  // doc["program"]["cycle"] = program.cycle();
  doc["program"]["isRunning"] = program.isRunning();
  // doc["program"]["timer"] = program.timer();
  // doc["program"]["pause"] = program.pause();

  String data;
  serializeJson(doc, data);

  events.send(data.c_str(), "status", now);
}
