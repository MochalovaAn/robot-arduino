void handleImuRoll(AsyncWebServerRequest *request)
{
  if (captivePortal(request))
    return;

  if (request->method() == HTTP_GET)
  {
    String content = String(imu.roll);
    request->send(200, "text/plain", content.c_str());

    Serial.println(200);
  }
  else
    methodNotAllowed(request, "GET");
}

void handleImuPitch(AsyncWebServerRequest *request)
{
  if (captivePortal(request))
    return;

  if (request->method() == HTTP_GET)
  {
    String content = String(imu.pitch);
    request->send(200, "text/plain", content.c_str());

    Serial.println(200);
  }
  else
    methodNotAllowed(request, "GET");
}

void handleImuYaw(AsyncWebServerRequest *request)
{
  if (captivePortal(request))
    return;

  if (request->method() == HTTP_GET)
  {
    String content = String(imu.yaw);
    request->send(200, "text/plain", content.c_str());

    Serial.println(200);
  }
  else
    methodNotAllowed(request, "GET");
}

void handleImuTemperature(AsyncWebServerRequest *request)
{
  if (captivePortal(request))
    return;

  if (request->method() == HTTP_GET)
  {
    String content = String(imu.t);
    request->send(200, "text/plain", content.c_str());

    Serial.println(200);
  }
  else
    methodNotAllowed(request, "GET");
}

