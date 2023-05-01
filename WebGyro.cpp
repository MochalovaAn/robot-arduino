#include "Web.h"

void handleGyro()
{
  if (handleCaptivePortal())
    return;

  if (webServer.method() == HTTP_GET)
  {
    DynamicJsonDocument doc(128);
    
    doc["deviceID"] = gyro.getDeviceID();
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

    webServer.send(200, "application/json", content.c_str());
    Serial.println(200);
  }
  else
    methodNotAllowed("GET");
}
