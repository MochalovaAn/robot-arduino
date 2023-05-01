#include <ESP8266WebServer.h>

void handleStepperAcceleration()
{
  if (captivePortal())
    return;

  if (webServer.method() == HTTP_GET)
  {
    float acceleration = stepper.acceleration();
    String message = String(acceleration);
    webServer.send(200, "text/plain", message.c_str());

    Serial.println(200);
  }
  else if (webServer.method() == HTTP_POST)
  {
    if (!webServer.hasArg("value"))
    {
      badRequest();
      return;
    }

    float acceleration = webServer.arg("value").toFloat();
    stepper.setAcceleration(acceleration);
    webServer.send(200, "text/plain", "");

    Serial.println(200);
  }
  else
    methodNotAllowed("GET, POST");
}

void handleStepperCurrentPosition()
{
  if (captivePortal())
    return;

  if (webServer.method() == HTTP_GET)
  {
    long position = stepper.currentPosition();
    String message = String(position);
    webServer.send(200, "text/plain", message.c_str());
    Serial.println(200);
  }
  else
    methodNotAllowed("GET");
}

void handleStepperCurrentSpeed()
{
  if (captivePortal())
    return;

  if (webServer.method() == HTTP_GET)
  {
    float speed = stepper.speed();
    String message = String(speed);
    webServer.send(200, "text/plain", message.c_str());
    Serial.println(200);
  }
  else
    methodNotAllowed("GET");
}

void handleStepperDisable()
{
  if (captivePortal())
    return;

  if (webServer.method() == HTTP_POST)
  {
    stepper.disableOutputs();
    webServer.send(200, "text/plain", "");
    Serial.println(200);
  }
  else
    methodNotAllowed("POST");
}

void handleStepperDistanceToGo()
{
  if (captivePortal())
    return;

  if (webServer.method() == HTTP_GET)
  {
    long distance = stepper.distanceToGo();
    String message = String(distance);
    webServer.send(200, "text/plain", message.c_str());
    Serial.println(200);
  }
  else
    methodNotAllowed("GET");
}

void handleStepperEnable()
{
  if (captivePortal())
    return;

  if (webServer.method() == HTTP_POST)
  {
    stepper.enableOutputs();
    webServer.send(200, "text/plain", "");
    Serial.println(200);
  }
  else
    methodNotAllowed("POST");
}

void handleStepperIsRunning()
{
  if (captivePortal())
    return;

  if (webServer.method() == HTTP_GET)
  {
    int state = stepper.isRunning() ? 1 : 0;
    String message = String(state);
    webServer.send(200, "text/plain", message.c_str());
    Serial.println(200);
  }
  else
    methodNotAllowed("GET");
}

void handleStepperMove()
{
  if (captivePortal())
    return;

  if (webServer.method() == HTTP_POST)
  {
    if (!webServer.hasArg("value"))
    {
      badRequest();
      return;
    }

    long relative = webServer.arg("value").toInt();
    stepper.move(relative);
    webServer.send(200, "text/plain", "");
    Serial.println(200);
  }
  else
    methodNotAllowed("POST");
}

void handleStepperMoveTo()
{
  if (captivePortal())
    return;

  if (webServer.method() == HTTP_POST)
  {
    if (!webServer.hasArg("value"))
    {
      badRequest();
      return;
    }

    long absolute = webServer.arg("value").toInt();
    stepper.moveTo(absolute);
    webServer.send(200, "text/plain", "");
    Serial.println(200);
  }
  else
    methodNotAllowed("POST");
}

void handleStepperReset()
{
  if (captivePortal())
    return;

  if (webServer.method() == HTTP_POST)
  {
    long position = webServer.arg("value").toInt();
    stepper.setCurrentPosition(position);
    webServer.send(200, "text/plain", "");
    Serial.println(200);
  }
  else
    methodNotAllowed("POST");
}

void handleStepperSpeed()
{
  if (captivePortal())
    return;

  if (webServer.method() == HTTP_GET)
  {
    float speed = stepper.maxSpeed();
    String message = String(speed);
    webServer.send(200, "text/plain", message.c_str());
    Serial.print(200);
  }
  else if (webServer.method() == HTTP_POST)
  {
    if (!webServer.hasArg("value"))
    {
      badRequest();
      return;
    }

    float speed = webServer.arg("value").toFloat();
    stepper.setMaxSpeed(speed);
    webServer.send(200, "text/plain", "");
    Serial.println(200);
  }
  else
    methodNotAllowed("GET, POST");
}

void handleStepperStepsPerRevolution()
{
  if (captivePortal())
    return;

  if (webServer.method() == HTTP_GET)
  {
    String message = "4096";
    webServer.send(200, "text/plain", message.c_str());
    Serial.println(200);
  }
  else
    methodNotAllowed("GET");
}

void handleStepperStop()
{
  if (captivePortal())
    return;

  if (webServer.method() == HTTP_POST)
  {
    stepper.stop();
    webServer.send(200, "text/plain", "");
    Serial.println(200);
  }
  else
    methodNotAllowed("POST");
}

void handleStepperTargetPosition()
{
  if (captivePortal())
    return;

  if (webServer.method() == HTTP_GET)
  {
    long position = stepper.targetPosition();
    String message = String(position);
    webServer.send(200, "text/plain", message.c_str());
    Serial.println(200);
  }
  else
    methodNotAllowed("GET");
}
