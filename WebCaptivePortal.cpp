#include "Web.h"

bool handleCaptivePortal()
{
  logRequest(false);

  if (webServer.hostHeader() != WiFi.softAPIP().toString())
  {
    String redirectHost = WiFi.softAPIP().toString();

    webServer.sendHeader("Location", F("http://") + redirectHost);
    webServer.send(302, "text/plain", "");

    Serial.print(302);
    Serial.print(" (");
    Serial.print(webServer.hostHeader());
    Serial.print(" -> ");
    Serial.print(redirectHost);
    Serial.println(")");

    return true;
  }

  return false;
}
