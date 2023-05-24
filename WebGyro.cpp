#include "Web.h"

void handleGyro(AsyncWebServerRequest *request)
{
  if (handleCaptivePortal(request))
    return;

  if (request->method() == HTTP_GET)
  {
    DynamicJsonDocument doc(128);
    
    doc["deviceID"] = gyro.deviceID;
    doc["roll"] = gyro.roll;
    doc["yaw"] = gyro.yaw;
    doc["pitch"] = gyro.pitch;
    doc["ax"] = gyro.ax;
    doc["ay"] = gyro.ay;
    doc["az"] = gyro.az;
    doc["gx"] = gyro.gx;
    doc["gy"] = gyro.gy;
    doc["gz"] = gyro.gz;
    doc["t"] = gyro.t;

    String content;
    serializeJson(doc, content);

    request->send(200, "application/json", content.c_str());
    Serial.println(200);
  }
  else
    methodNotAllowed(request, "GET");
}
