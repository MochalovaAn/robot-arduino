#ifndef GYRO_H
#define GYRO_H

#include <Wire.h>
#include <MPU6050.h>
#include <MadgwickAHRS.h>

// по умолчанию драйвер MPU6050 настраивает акселерометр/гироскоп
// на самую высокую чувствительность +/- 2g и +/- 250 град/сек

#define ACCEL_RAW_TO_FS(x) ((x)*2.0) / 32768.0
#define GYRO_RAW_TO_FS(x) ((x)*250.0) / 32768.0
#define TEMP_RAW_TO_FS(x) ((x) / 340.0) + 36.53

class Gyro
{
private:
  MPU6050 mpu;
  Madgwick filter;
  unsigned long period, timer;

public:
  uint8_t deviceID;
  float ax, ay, az, gx, gy, gz, t;
  float roll, pitch, yaw;

  bool init(unsigned long _period);
  bool getMotion();
  uint8_t getDeviceID() { return mpu.getDeviceID(); };
};

#endif // GYRO_H
