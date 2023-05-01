void handleStepperAcceleration()
{
  if (handleCaptivePortal())
    return;

  if (webServer.method() == HTTP_GET)
  {
    String content = String(stepper.acceleration());

    webServer.send(200, "text/plain", content);
    Serial.println(200);
  }
  else if (webServer.method() == HTTP_POST)
  {
    if (webServer.args() != 1)
    {
      badRequest();
      return;
    }

    float acceleration = webServer.arg(0).toFloat();
    stepper.setAcceleration(acceleration);

    webServer.send(200, "text/plain", "");
    Serial.println(200);
  }
  else
    methodNotAllowed("GET, POST");
}

void handleStepperCurrentPosition()
{
  if (handleCaptivePortal())
    return;

  if (webServer.method() == HTTP_GET)
  {
    String content = String(stepper.currentPosition());

    webServer.send(200, "text/plain", content.c_str());
    Serial.println(200);
  }
  else
    methodNotAllowed("GET");
}

void handleStepperCurrentSpeed()
{
  if (handleCaptivePortal())
    return;

  if (webServer.method() == HTTP_GET)
  {
    String content = String(stepper.speed());

    webServer.send(200, "text/plain", content.c_str());
    Serial.println(200);
  }
  else
    methodNotAllowed("GET");
}

void handleStepperDisable()
{
  if (handleCaptivePortal())
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
  if (handleCaptivePortal())
    return;

  if (webServer.method() == HTTP_GET)
  {
    String content = String(stepper.distanceToGo());

    webServer.send(200, "text/plain", content.c_str());
    Serial.println(200);
  }
  else
    methodNotAllowed("GET");
}

void handleStepperEnable()
{
  if (handleCaptivePortal())
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
  if (handleCaptivePortal())
    return;

  if (webServer.method() == HTTP_GET)
  {
    String content = String((int)stepper.isRunning());

    webServer.send(200, "text/plain", content.c_str());
    Serial.println(200);
  }
  else
    methodNotAllowed("GET");
}

void handleStepperMove()
{
  if (handleCaptivePortal())
    return;

  if (webServer.method() == HTTP_POST)
  {
    if (webServer.hasArg("relative"))
    {
      long relative = webServer.arg("relative").toInt();
      stepper.move(relative);
    }
    else if (webServer.hasArg("absolute"))
    {
      long absolute = webServer.arg("absolute").toInt();
      stepper.moveTo(absolute);
    }
    else
    {
      badRequest();
      return;
    }

    webServer.send(200, "text/plain", "");
    Serial.println(200);
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

    webServer.send(200, "text/plain", "");
    Serial.println(200);
  }
  else
    methodNotAllowed("POST");
}

void handleStepperSpeed()
{
  if (handleCaptivePortal())
    return;

  if (webServer.method() == HTTP_GET)
  {
    String content = String(stepper.maxSpeed());

    webServer.send(200, "text/plain", content.c_str());
    Serial.print(200);
  }
  else if (webServer.method() == HTTP_POST)
  {
    if (webServer.args() != 1)
    {
      badRequest();
      return;
    }

    float speed = webServer.arg(0).toFloat();
    stepper.setMaxSpeed(speed);

    webServer.send(200, "text/plain", "");
    Serial.println(200);
  }
  else
    methodNotAllowed("GET, POST");
}

void handleStepperStepsPerRevolution()
{
  if (handleCaptivePortal())
    return;

  if (webServer.method() == HTTP_GET)
  {
    String content = "2048";

    webServer.send(200, "text/plain", content.c_str());
    Serial.println(200);
  }
  else
    methodNotAllowed("GET");
}

void handleStepperStop()
{
  if (handleCaptivePortal())
    return;

  if (webServer.method() == HTTP_POST)
  {
    int force = webServer.arg("force").toInt();
    force ? stepper.setCurrentPosition(stepper.currentPosition()) : stepper.stop();

    webServer.send(200, "text/plain", "");
    Serial.println(200);
  }
  else
    methodNotAllowed("POST");
}

void handleStepperTargetPosition()
{
  if (handleCaptivePortal())
    return;

  if (webServer.method() == HTTP_GET)
  {
    String content = String(stepper.targetPosition());

    webServer.send(200, "text/plain", content.c_str());
    Serial.println(200);
  }
  else
    methodNotAllowed("GET");
}
