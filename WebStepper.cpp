#include "Web.h"
#include "Stepper.h"

void handleStepper(AsyncWebServerRequest *request)
{
  if (handleCaptivePortal(request))
    return;

  if (request->method() == HTTP_GET)
  {
    DynamicJsonDocument doc(1024);

    doc["acceleration"] = Stepper.acceleration();
    doc["speed"] = Stepper.maxSpeed();
    doc["currentSpeed"] = Stepper.speed();
    doc["currentPosition"] = Stepper.currentPosition();
    doc["targetPosition"] = Stepper.targetPosition();
    doc["distanceToGo"] = Stepper.distanceToGo();
    doc["isRunning"] = Stepper.isRunning();
    doc["stepsPerRevolution"] = STEPS_PER_REVOLUTION;

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
      Stepper.setAcceleration(acceleration);
    }

    if (request->hasArg("speed"))
    {
      float speed = request->arg("speed").toFloat();
      Stepper.setMaxSpeed(speed);
    }

    if (request->hasArg("rotate"))
    {
      long relative = request->arg("rotate").toInt();
      Stepper.move(relative);
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
      Stepper.move(relative);

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
    force ? Stepper.setCurrentPosition(Stepper.currentPosition()) : Stepper.stop();

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
    Stepper.setCurrentPosition(position);

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
    Stepper.enableOutputs();

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
    Stepper.disableOutputs();

    request->send(204);
    Serial.println(204);
  }
  else
    methodNotAllowed(request, "POST");
}
