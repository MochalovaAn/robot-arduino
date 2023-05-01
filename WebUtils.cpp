#include "Web.h"

void logRequest(bool eol)
{
  char *httpMethods[] = {"ANY", "GET", "HEAD", "POST", "PUT", "PATCH", "DELETE", "OPTIONS"};

  Serial.print(webServer.client().remoteIP());
  Serial.print(" - \"");
  Serial.print(httpMethods[webServer.method()]);
  Serial.print(" ");
  Serial.print(webServer.uri());

  if (webServer.args() > 0)
  {
    int count = 0;
    for (int i = 0; i < webServer.args(); i++)
    {
      if (webServer.argName(i) == "plain")
        break;
      Serial.print(count ? "&" : "?");
      Serial.print(webServer.argName(i));
      Serial.print("=");
      Serial.print(webServer.arg(i));
      count++;
    }
  }

  Serial.print("\" ");

  if (eol)
    Serial.println();
}

void badRequest()
{
  webServer.send(400, "text/plain", "400 Bad Request");
  Serial.println(400);
}

void methodNotAllowed(String allow)
{
  webServer.sendHeader("Allow", allow);
  webServer.send(405, "text/plain", "405 Method Not Allowed");
  Serial.println(405);
}
