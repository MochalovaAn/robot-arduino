#include "Web.h";

void handleNotFound()
{
  if (handleCaptivePortal())
    return;

  webServer.send(404, "text/plain", "Not Found");
  Serial.println(404);
}
