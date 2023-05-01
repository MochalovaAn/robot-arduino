#include "Web.h"

void handleStepper()
{
  if (handleCaptivePortal())
    return;

  if (webServer.method() == HTTP_GET)
  {
    DynamicJsonDocument doc(1024);

    doc["acceleration"] = stepper.acceleration();
    doc["speed"] = stepper.maxSpeed();
    doc["currentSpeed"] = stepper.speed();
    doc["currentPosition"] = stepper.currentPosition();
    doc["targetPosition"] = stepper.targetPosition();
    doc["distanceToGo"] = stepper.distanceToGo();
    doc["isRunning"] = stepper.isRunning();
    doc["stepsPerRevolution"] = 2048;

    String content;
    serializeJson(doc, content);

    webServer.send(200, "application/json", content);
    Serial.println(200);
  }
  else if (webServer.method() == HTTP_POST)
  {
    if (webServer.hasArg("acceleration"))
    {
      float acceleration = webServer.arg("acceleration").toFloat();
      stepper.setAcceleration(acceleration);
    }

    if (webServer.hasArg("speed"))
    {
      float speed = webServer.arg("speed").toFloat();
      stepper.setMaxSpeed(speed);
    }

    webServer.send(204);
    Serial.println(204);
  }
  else
    methodNotAllowed("GET, POST");
}

void handleStepperRotate()
{
  if (handleCaptivePortal())
    return;

  if (webServer.method() == HTTP_POST)
  {
    if (webServer.hasArg("steps"))
    {
      long relative = webServer.arg("steps").toInt();
      stepper.move(relative);
    }

    webServer.send(204);
    Serial.println(204);
  }
  else
    methodNotAllowed("POST");
}

void handleStepperStop()
{
  if (handleCaptivePortal())
    return;

  if (webServer.method() == HTTP_POST)
  {
    int force = webServer.arg("force").toInt();
    force ? stepper.setCurrentPosition(stepper.currentPosition()) : stepper.stop();

    webServer.send(204);
    Serial.println(204);
  }
  else
    methodNotAllowed("POST");
}

void handleStepperReset()
{
  if (handleCaptivePortal())
    return;

  if (webServer.method() == HTTP_POST)
  {
    long position = webServer.arg("position").toInt();
    stepper.setCurrentPosition(position);

    webServer.send(204);
    Serial.println(204);
  }
  else
    methodNotAllowed("POST");
}
