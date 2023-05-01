void handleNotFound(AsyncWebServerRequest *request)
{
  if (captivePortal(request))
    return;

  request->send(404, "text/plain", "Not Found");
  Serial.println(404);
}
