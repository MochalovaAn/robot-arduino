#include "Web.h"

void handleStepper(AsyncWebServerRequest *request)
{
  if (handleCaptivePortal(request))
    return;

  if (request->method() == HTTP_GET)
  {
    DynamicJsonDocument doc(1024);

    doc["acceleration"] = stepper.acceleration();
    doc["speed"] = stepper.maxSpeed();
    doc["currentSpeed"] = stepper.speed();
    doc["currentPosition"] = stepper.currentPosition();
    doc["targetPosition"] = stepper.targetPosition();
    doc["distanceToGo"] = stepper.distanceToGo();
    doc["isRunning"] = stepper.isRunning();
    doc["stepsPerRevolution"] = stepper.stepsPerRevolution();

    String content;
    serializeJson(doc, content);

    request->send(200, "application/json", content);
    Serial.println(200);
  }
  else if (request->method() == HTTP_POST)
  {
    if (request->hasArg("acceleration"))
    {
      float acceleration = request->arg("acceleration").toFloat();
      stepper.setAcceleration(acceleration);
    }

    if (request->hasArg("speed"))
    {
      float speed = request->arg("speed").toFloat();
      stepper.setMaxSpeed(speed);
    }

    if (request->hasArg("rotate"))
    {
      long relative = request->arg("rotate").toInt();
      stepper.move(relative);
    }

    request->send(204);
    Serial.println(204);
  }
  else
    methodNotAllowed(request, "GET, POST");
}

void handleStepperRotate(AsyncWebServerRequest *request)
{
  if (handleCaptivePortal(request))
    return;

  if (request->method() == HTTP_POST)
  {
    Serial.print(request->args());
    Serial.print(" ");

    if (request->hasArg("steps"))
    {
      long relative = request->arg("steps").toInt();
      stepper.move(relative);

      Serial.print(relative);
      Serial.print(" ");
    }

    request->send(204);
    Serial.println(204);
  }
  else
    methodNotAllowed(request, "POST");
}

void handleStepperStop(AsyncWebServerRequest *request)
{
  if (handleCaptivePortal(request))
    return;

  if (request->method() == HTTP_POST)
  {
    int force = request->arg("force").toInt();
    force ? stepper.setCurrentPosition(stepper.currentPosition()) : stepper.stop();

    request->send(204);
    Serial.println(204);
  }
  else
    methodNotAllowed(request, "POST");
}

void handleStepperReset(AsyncWebServerRequest *request)
{
  if (handleCaptivePortal(request))
    return;

  if (request->method() == HTTP_POST)
  {
    long position = request->arg("position").toInt();
    stepper.setCurrentPosition(position);

    request->send(204);
    Serial.println(204);
  }
  else
    methodNotAllowed(request, "POST");
}

void handleStepperEnable(AsyncWebServerRequest *request)
{
  if (handleCaptivePortal(request))
    return;

  if (request->method() == HTTP_POST)
  {
    stepper.enableOutputs();

    request->send(204);
    Serial.println(204);
  }
  else
    methodNotAllowed(request, "POST");
}

void handleStepperDisable(AsyncWebServerRequest *request)
{
  if (handleCaptivePortal(request))
    return;

  if (request->method() == HTTP_POST)
  {
    stepper.disableOutputs();

    request->send(204);
    Serial.println(204);
  }
  else
    methodNotAllowed(request, "POST");
}
