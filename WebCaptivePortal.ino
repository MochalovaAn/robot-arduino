#include <ESP8266WiFi.h>

void handleRoot()
{
  if (captivePortal())
    return;

  if (webServer.method() != HTTP_GET)
  {
    methodNotAllowed("GET");
    return;
  }

  String page;
  page = F(
      "<!DOCTYPE html><html lang='en'>"
      "<head>"
      "<meta name='viewport' content='width=device-width'>"
      "<title>Robot</title>"
      "</head>"
      "<body>"
      "<h1>Robot</h1>"
      "<div>Welcome to the balance-controlled cylindrical robot portal</div>"
      "</body>"
      "</html>");

  webServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  webServer.sendHeader("Pragma", "no-cache");
  webServer.sendHeader("Expires", "-1");
  webServer.send(200, "text/html", page);

  Serial.println(200);
}

bool captivePortal()
{
  char *httpMethods[] = {"ANY", "GET", "HEAD", "POST", "PUT", "PATCH", "DELETE", "OPTIONS"};

  Serial.print(webServer.client().remoteIP());
  Serial.print(" - \"");
  Serial.print(httpMethods[webServer.method()]);
  Serial.print(" ");
  Serial.print(webServer.uri());

  if (webServer.args() > 0)
  {
    Serial.print("?");
    for (int i = 0; i < webServer.args(); i++)
    {
      if (!i) Serial.print("&");
      Serial.print(webServer.argName(i));
      Serial.print("=");
      Serial.print(webServer.arg(i));
    }
  }

  Serial.print("\" ");

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
