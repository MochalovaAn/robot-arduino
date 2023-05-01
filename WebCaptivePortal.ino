bool captivePortal(AsyncWebServerRequest *request)
{
  Serial.print(request->client()->remoteIP());
  Serial.print(" - \"");
  Serial.print(httpMethod(request->method()));
  Serial.print(" ");
  Serial.print(request->url());

  if (request->args() > 0)
  {
    Serial.print("?");
    for (int i = 0; i < request->args(); i++)
    {
      if (!i)
        Serial.print("&");
      Serial.print(request->argName(i));
      Serial.print("=");
      Serial.print(request->arg(i));
    }
  }

  Serial.print("\" ");

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

void handleCaptivePortal(AsyncWebServerRequest *request)
{
  if (captivePortal(request))
    return;

  if (request->method() != HTTP_GET)
  {
    methodNotAllowed(request, "GET");
    return;
  }

  AsyncResponseStream *response = request->beginResponseStream("text/html");
  response->print(
      "<!DOCTYPE html><html lang='en'>"
      "<head>"
      "<meta name='viewport' content='width=device-width'>"
      "<title>Robot</title>"
      "</head>"
      "<body>"
      "<h1>Robot</h1>"
      "<div>Welcome to the Robot Captive Portal</div>"
      "<small id='test'></small>"
      "<script>"
      "if (!!window.EventSource) {"
      "var source = new EventSource('/events');"
      "source.addEventListener('open', function(e) { console.log('Robot Events Connected'); }, false);"
      "source.addEventListener('error', function(e) { if (e.target.readyState != EventSource.OPEN) { console.log('Robot Events Disconnected'); document.getElementById('test').innerHTML = 'Disconnected'; } }, false);"
      "source.addEventListener('test', function(e) { document.getElementById('test').innerHTML = e.data; }, false);"
      "}"
      "</script>"
      "</body>"
      "</html>");
  response->addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  response->addHeader("Pragma", "no-cache");
  response->addHeader("Expires", "-1");

  request->send(response);

  Serial.println(200);
}
