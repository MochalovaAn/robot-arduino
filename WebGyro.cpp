#include "Web.h"
#include "Gyro.h"

void handleGyro(AsyncWebServerRequest *request)
{
  if (handleCaptivePortal(request))
    return;

  if (request->method() == HTTP_GET)
  {
    DynamicJsonDocument doc(128);
    
    doc["deviceID"] = Gyro.deviceID;
    doc["roll"] = Gyro.roll;
    doc["yaw"] = Gyro.yaw;
    doc["pitch"] = Gyro.pitch;

    String content;
    serializeJson(doc, content);

    request->send(200, "application/json", content.c_str());
    Serial.println(200);
  }
  else
    methodNotAllowed(request, "GET");
}
