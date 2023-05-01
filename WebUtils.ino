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

void badRequest(AsyncWebServerRequest *request)
{
  request->send(400, "text/plain", "400 Bad Request");
  Serial.println(400);
}

void methodNotAllowed(AsyncWebServerRequest *request, String allow)
{
  AsyncWebServerResponse *response = request->beginResponse(405, "text/plain", "405 Method Not Allowed");
  response->addHeader("Allow", allow);
  request->send(response);
  Serial.println(405);
}
