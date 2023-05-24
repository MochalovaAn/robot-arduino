#include "Web.h"

void handleProgram(AsyncWebServerRequest *request)
{
  if (handleCaptivePortal(request))
    return;

  if (request->method() == HTTP_GET)
  {
    DynamicJsonDocument doc(128);

    doc["state"] = program.state();
    doc["line"] = program.line();
    doc["count"] = program.count();
    doc["cycle"] = program.cycle();
    doc["isRunning"] = program.isRunning();
    doc["timer"] = program.timer();
    doc["pause"] = program.pause();

    String content;
    serializeJson(doc, content);

    request->send(200, "application/json", content.c_str());
    Serial.println(200);
  }
  else
    methodNotAllowed(request, "GET");
}

void handleProgramText(AsyncWebServerRequest *request)
{
  if (handleCaptivePortal(request))
    return;

  if (request->method() == HTTP_GET)
  {
    String content = program.getAsText();

    request->send(200, "text/plain", content);
    Serial.println(200);
  }
  else if (request->method() == HTTP_POST)
  {
    String text = "";

    if (request->hasArg("text"))
    {
      text = request->arg("text");
    }

    request->send(204);
    Serial.println(204);

    program.setAsText(text);
  }
  else
    methodNotAllowed(request, "GET, POST");
}

void handleProgramRun(AsyncWebServerRequest *request)
{
  if (handleCaptivePortal(request))
    return;

  if (request->method() == HTTP_POST)
  {
    request->send(204);
    Serial.println(204);

    program.run();
  }
  else
    methodNotAllowed(request, "POST");
}

void handleProgramStop(AsyncWebServerRequest *request)
{
  if (handleCaptivePortal(request))
    return;

  if (request->method() == HTTP_POST)
  {
    int force = request->arg("force").toInt();

    request->send(204);
    Serial.println(204);

    program.stop(force != 0);
  }
  else
    methodNotAllowed(request, "POST");
}
