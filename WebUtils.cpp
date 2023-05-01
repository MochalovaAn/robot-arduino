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

void notFound()
{
  webServer.send(404, "text/plain", "404 Not Found");
  Serial.println(404);
}

void methodNotAllowed(String allow)
{
  webServer.sendHeader("Allow", allow);
  webServer.send(405, "text/plain", "405 Method Not Allowed");
  Serial.println(405);
}

void internalServerError()
{
  webServer.send(500, "text/plain", "500 Internal Server Error");
  Serial.println(500);
}
