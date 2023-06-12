/*
  This class reads yaw/roll/pitch data from the MPU6060

  Based on MPU6050_DMP6 example for MPU6050 library by Electronic Cats
  https://github.com/sponsors/ElectronicCats

  GY-521  NodeMCU       Description
  ======= ==========    ====================================================
  VCC     VU (5V USB)   Not available on all boards so use 3.3V if needed.
  GND     G             Ground
  SCL     D1 (GPIO5)    I2C clock
  SDA     D2 (GPIO4)    I2C data
  XDA     not connected
  XCL     not connected
  AD0     not connected
  INT     not connected
*/

#ifndef GYRO_H
#define GYRO_H

#include <Arduino.h>

class GyroClass
{
private:
  bool dmpReady;

public:
  uint8_t deviceID;
  float roll, pitch, yaw;

  bool init();
  bool getMotion();
  uint8_t getDeviceID() { return deviceID; };
};

extern GyroClass Gyro;

#endif // GYRO_H
