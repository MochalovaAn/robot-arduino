void handleRoot()
{
  if (handleCaptivePortal())
    return;

  if (webServer.method() != HTTP_GET)
  {
    methodNotAllowed("GET");
    return;
  }

  String content;
  content = F(
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

  webServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  webServer.sendHeader("Pragma", "no-cache");
  webServer.sendHeader("Expires", "-1");
  webServer.send(200, "text/html", content);

  Serial.println(200);
}
