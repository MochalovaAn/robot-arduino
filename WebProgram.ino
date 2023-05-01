void handleProgramText()
{
  if (handleCaptivePortal())
    return;

  if (webServer.method() == HTTP_GET)
  {
    String content = program.getAsText();

    webServer.send(200, "text/plain", content);
    Serial.println(200);
  }
  else if (webServer.method() == HTTP_POST)
  {
    String text;

    if (webServer.header("Content-Type") == "text/plain")
    {
      text = webServer.arg("plain");
    }
    else if (webServer.hasArg("text"))
    {
      text = webServer.arg("text");
    }
    else
    {
      badRequest();
      return;
    }

    webServer.send(200, "text/plain", "");
    Serial.println(200);

    program.setAsText(text);
  }
  else
    methodNotAllowed("GET, POST");
}

void handleProgramStart()
{
  if (handleCaptivePortal())
    return;

  if (webServer.method() == HTTP_POST)
  {
    webServer.send(200, "text/plain", "");
    Serial.println(200);

    program.start();
  }
  else
    methodNotAllowed("POST");
}

void handleProgramStop()
{
  if (handleCaptivePortal())
    return;

  if (webServer.method() == HTTP_POST)
  {
    int force = webServer.arg("force").toInt();

    webServer.send(200, "text/plain", "");
    Serial.println(200);

    program.stop(force != 0);
  }
  else
    methodNotAllowed("POST");
}

void handleProgramState()
{
  if (handleCaptivePortal())
    return;

  if (webServer.method() == HTTP_GET)
  {
    String content = String(program.state());

    webServer.send(200, "text/plain", content.c_str());
    Serial.println(200);
  }
  else
    methodNotAllowed("GET");
}

void handleProgramLine()
{
  if (handleCaptivePortal())
    return;

  if (webServer.method() == HTTP_GET)
  {
    String content = String(program.line());

    webServer.send(200, "text/plain", content.c_str());
    Serial.println(200);
  }
  else
    methodNotAllowed("GET");
}

void handleProgramIsRunning()
{
  if (handleCaptivePortal())
    return;

  if (webServer.method() == HTTP_GET)
  {
    String content = String((int)program.isRunning());

    webServer.send(200, "text/plain", content.c_str());
    Serial.println(200);
  }
  else
    methodNotAllowed("GET");
}
