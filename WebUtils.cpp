#include "Web.h"

char *httpMethod(WebRequestMethodComposite method)
{
  switch (method)
  {
    case HTTP_GET: return "GET";
    case HTTP_POST: return "POST";
    case HTTP_DELETE: return "DELETE";
    case HTTP_PUT: return "PUT";
    case HTTP_PATCH: return "PATCH";
    case HTTP_HEAD: return "HEAD";
    case HTTP_OPTIONS: return "OPTIONS";
  }
  return "ANY";
}

void logRequest(AsyncWebServerRequest *request, bool eol)
{
  Serial.print(request->client()->remoteIP());
  Serial.print(" - \"");
  Serial.print(httpMethod(request->method()));
  Serial.print(" ");
  Serial.print(request->url());

  if (request->args() > 0)
  {
    int count = 0;
    for (int i = 0; i < request->args(); i++)
    {
      if (request->argName(i) == "plain")
        break;
      Serial.print(count ? "&" : "?");
      Serial.print(request->argName(i));
      Serial.print("=");
      Serial.print(request->arg(i));
      count++;
    }
  }

  Serial.print("\" ");

  if (eol)
    Serial.println();
}

void badRequest(AsyncWebServerRequest *request)
{
  request->send(400, "text/plain", "400 Bad Request");
  Serial.println(400);
}

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "404 Not Found");
  Serial.println(404);
}

void methodNotAllowed(AsyncWebServerRequest *request, String allow)
{
  AsyncWebServerResponse *response = request->beginResponse(405, "text/plain", "405 Method Not Allowed");
  response->addHeader("Allow", allow);
  request->send(response);
  Serial.println(405);
}

void internalServerError(AsyncWebServerRequest *request)
{
  request->send(500, "text/plain", "500 Internal Server Error");
  Serial.println(500);
}
