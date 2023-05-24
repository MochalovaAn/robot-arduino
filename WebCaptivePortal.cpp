#include "Web.h"

bool handleCaptivePortal(AsyncWebServerRequest *request)
{
  logRequest(request, false);

  if (request->header("Host") != WiFi.softAPIP().toString())
  {
    String redirectHost = WiFi.softAPIP().toString();

    request->redirect(F("http://") + redirectHost);

    Serial.print(302);
    Serial.print(" (");
    Serial.print(request->header("Host"));
    Serial.print(" -> ");
    Serial.print(redirectHost);
    Serial.println(")");

    return true;
  }

  return false;
}
