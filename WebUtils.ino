void badRequest()
{
  webServer.send(400, "text/plain", "400 Bad Request");
  Serial.println(400);
}

void methodNotAllowed(const char *allow)
{
  webServer.sendHeader("Allow", allow);
  webServer.send(405, "text/plain", "405 Method Not Allowed");
  Serial.println(405);
}
