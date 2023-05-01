#include "Web.h"

void handleProgram()
{
  if (handleCaptivePortal())
    return;

  if (webServer.method() == HTTP_GET)
  {
    DynamicJsonDocument doc(128);

    doc["state"] = program.state();
    doc["cycle"] = program.cycle();
    doc["line"] = program.line();
    doc["isRunning"] = program.isRunning();

    String content;
    serializeJson(doc, content);

    webServer.send(200, "application/json", content.c_str());
    Serial.println(200);
  }
  else
    methodNotAllowed("GET");
}

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
    String text = "";

    if (webServer.header("Content-Type") == "text/plain")
    {
      text = webServer.arg("plain");
    }
    else if (webServer.hasArg("text"))
    {
      text = webServer.arg("text");
    }

    webServer.send(204);
    Serial.println(204);

    program.setAsText(text);
  }
  else
    methodNotAllowed("GET, POST");
}

void handleProgramRun()
{
  if (handleCaptivePortal())
    return;

  if (webServer.method() == HTTP_POST)
  {
    webServer.send(204);
    Serial.println(204);

    program.run();
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

    webServer.send(204);
    Serial.println(204);

    program.stop(force != 0);
  }
  else
    methodNotAllowed("POST");
}
