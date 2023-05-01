void handleStepperAcceleration(AsyncWebServerRequest *request)
{
  if (captivePortal(request))
    return;

  if (request->method() == HTTP_GET)
  {
    float acceleration = stepper.acceleration();
    String content = String(acceleration);
    request->send(200, "text/plain", content.c_str());

    Serial.println(200);
  }
  else if (request->method() == HTTP_POST)
  {
    if (!request->hasArg("value"))
    {
      badRequest(request);
      return;
    }

    float acceleration = request->arg("value").toFloat();
    stepper.setAcceleration(acceleration);
    request->send(200, "text/plain", "");

    Serial.println(200);
  }
  else
    methodNotAllowed(request, "GET, POST");
}

void handleStepperCurrentPosition(AsyncWebServerRequest *request)
{
  if (captivePortal(request))
    return;

  if (request->method() == HTTP_GET)
  {
    long position = stepper.currentPosition();
    String content = String(position);
    request->send(200, "text/plain", content.c_str());

    Serial.println(200);
  }
  else
    methodNotAllowed(request, "GET");
}

void handleStepperCurrentSpeed(AsyncWebServerRequest *request)
{
  if (captivePortal(request))
    return;

  if (request->method() == HTTP_GET)
  {
    float speed = stepper.speed();
    String content = String(speed);
    request->send(200, "text/plain", content.c_str());

    Serial.println(200);
  }
  else
    methodNotAllowed(request, "GET");
}

void handleStepperDisable(AsyncWebServerRequest *request)
{
  if (captivePortal(request))
    return;

  if (request->method() == HTTP_POST)
  {
    stepper.disableOutputs();
    request->send(200, "text/plain", "");

    Serial.println(200);
  }
  else
    methodNotAllowed(request, "POST");
}

void handleStepperDistanceToGo(AsyncWebServerRequest *request)
{
  if (captivePortal(request))
    return;

  if (request->method() == HTTP_GET)
  {
    long distance = stepper.distanceToGo();
    String content = String(distance);
    request->send(200, "text/plain", content.c_str());

    Serial.println(200);
  }
  else
    methodNotAllowed(request, "GET");
}

void handleStepperEnable(AsyncWebServerRequest *request)
{
  if (captivePortal(request))
    return;

  if (request->method() == HTTP_POST)
  {
    stepper.enableOutputs();
    request->send(200, "text/plain", "");

    Serial.println(200);
  }
  else
    methodNotAllowed(request, "POST");
}

void handleStepperIsRunning(AsyncWebServerRequest *request)
{
  if (captivePortal(request))
    return;

  if (request->method() == HTTP_GET)
  {
    int state = stepper.isRunning() ? 1 : 0;
    String content = String(state);
    request->send(200, "text/plain", content.c_str());

    Serial.println(200);
  }
  else
    methodNotAllowed(request, "GET");
}

void handleStepperMove(AsyncWebServerRequest *request)
{
  if (captivePortal(request))
    return;

  if (request->method() == HTTP_POST)
  {
    if (!request->hasArg("value"))
    {
      badRequest(request);
      return;
    }

    long relative = request->arg("value").toInt();
    stepper.move(relative);
    request->send(200, "text/plain", "");

    Serial.println(200);
  }
  else
    methodNotAllowed(request, "POST");
}

void handleStepperMoveTo(AsyncWebServerRequest *request)
{
  if (captivePortal(request))
    return;

  if (request->method() == HTTP_POST)
  {
    if (!request->hasArg("value"))
    {
      badRequest(request);
      return;
    }

    long absolute = request->arg("value").toInt();
    stepper.moveTo(absolute);
    request->send(200, "text/plain", "");

    Serial.println(200);
  }
  else
    methodNotAllowed(request, "POST");
}

void handleStepperReset(AsyncWebServerRequest *request)
{
  if (captivePortal(request))
    return;

  if (request->method() == HTTP_POST)
  {
    long position = request->arg("value").toInt();
    stepper.setCurrentPosition(position);
    request->send(200, "text/plain", "");

    Serial.println(200);
  }
  else
    methodNotAllowed(request, "POST");
}

void handleStepperSpeed(AsyncWebServerRequest *request)
{
  if (captivePortal(request))
    return;

  if (request->method() == HTTP_GET)
  {
    float speed = stepper.maxSpeed();
    String content = String(speed);
    request->send(200, "text/plain", content.c_str());

    Serial.print(200);
  }
  else if (request->method() == HTTP_POST)
  {
    if (!request->hasArg("value"))
    {
      badRequest(request);
      return;
    }

    float speed = request->arg("value").toFloat();
    stepper.setMaxSpeed(speed);
    request->send(200, "text/plain", "");

    Serial.println(200);
  }
  else
    methodNotAllowed(request, "GET, POST");
}

void handleStepperStepsPerRevolution(AsyncWebServerRequest *request)
{
  if (captivePortal(request))
    return;

  if (request->method() == HTTP_GET)
  {
    String content = "4096";
    request->send(200, "text/plain", content.c_str());

    Serial.println(200);
  }
  else
    methodNotAllowed(request, "GET");
}

void handleStepperStop(AsyncWebServerRequest *request)
{
  if (captivePortal(request))
    return;

  if (request->method() == HTTP_POST)
  {
    stepper.stop();
    request->send(200, "text/plain", "");

    Serial.println(200);
  }
  else
    methodNotAllowed(request, "POST");
}

void handleStepperTargetPosition(AsyncWebServerRequest *request)
{
  if (captivePortal(request))
    return;

  if (request->method() == HTTP_GET)
  {
    long position = stepper.targetPosition();
    String content = String(position);
    request->send(200, "text/plain", content.c_str());

    Serial.println(200);
  }
  else
    methodNotAllowed(request, "GET");
}
