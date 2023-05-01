void handleImuRoll()
{
  if (handleCaptivePortal())
    return;

  if (webServer.method() == HTTP_GET)
  {
    String content = String(imu.roll);

    webServer.send(200, "text/plain", content.c_str());
    Serial.println(200);
  }
  else
    methodNotAllowed("GET");
}

void handleImuPitch()
{
  if (handleCaptivePortal())
    return;

  if (webServer.method() == HTTP_GET)
  {
    String content = String(imu.pitch);

    webServer.send(200, "text/plain", content.c_str());
    Serial.println(200);
  }
  else
    methodNotAllowed("GET");
}

void handleImuYaw()
{
  if (handleCaptivePortal())
    return;

  if (webServer.method() == HTTP_GET)
  {
    String content = String(imu.yaw);

    webServer.send(200, "text/plain", content.c_str());
    Serial.println(200);
  }
  else
    methodNotAllowed("GET");
}

void handleImuTemperature()
{
  if (handleCaptivePortal())
    return;

  if (webServer.method() == HTTP_GET)
  {
    String content = String(imu.t);

    webServer.send(200, "text/plain", content.c_str());
    Serial.println(200);
  }
  else
    methodNotAllowed("GET");
}
