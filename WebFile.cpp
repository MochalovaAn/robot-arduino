#include "Extern.h"
#include "Web.h"

void handleFile()
{
  if (handleCaptivePortal())
    return;

  String path = webServer.uri();
  if (path == "/")
    path += "index.html";

  if (!LittleFS.exists(path))
  {
    notFound();
    return;
  }

  if (webServer.method() != HTTP_GET)
  {
    methodNotAllowed("GET");
    return;
  }

  File file = LittleFS.open(path, "r");
  if (!file)
  {
    internalServerError();
    return;
  }

  webServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  webServer.sendHeader("Pragma", "no-cache");
  webServer.sendHeader("Expires", "-1");
  webServer.streamFile(file, mime::getContentType(path));

  file.close();

  Serial.println(200);
}
