#include "Web.h"
#include "Program.h"

void handleProgram(AsyncWebServerRequest *request)
{
  if (handleCaptivePortal(request))
    return;

  if (request->method() == HTTP_GET)
  {
    DynamicJsonDocument doc(128);

    doc["state"] = Program.state();
    doc["line"] = Program.line();
    doc["count"] = Program.count();
    doc["cycle"] = Program.cycle();
    doc["isRunning"] = Program.isRunning();
    doc["timer"] = Program.timer();
    doc["pause"] = Program.pause();

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
    String content = Program.getAsText();

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

    Program.setAsText(text);
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

    Program.run();
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

    Program.stop(force != 0);
  }
  else
    methodNotAllowed(request, "POST");
}
