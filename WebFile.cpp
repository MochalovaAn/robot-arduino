#include "Extern.h"
#include "Web.h"

void handleFile(AsyncWebServerRequest *request)
{
  if (handleCaptivePortal(request))
    return;

  String path = request->url();
  if (path == "/")
    path += "index.html";

  if (!LittleFS.exists(path))
  {
    notFound(request);
    return;
  }

  if (request->method() != HTTP_GET)
  {
    methodNotAllowed(request, "GET");
    return;
  }

  request->send(LittleFS, path);

  // request->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  // request->sendHeader("Pragma", "no-cache");
  // request->sendHeader("Expires", "-1");
  // request->streamFile(file, mime::getContentType(path));

  Serial.println(200);
}
