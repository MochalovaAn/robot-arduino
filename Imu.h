#ifndef IMU_H
#define IMU_H

#include <Wire.h>
#include <MPU6050.h>
#include <MadgwickAHRS.h>

// по умолчанию драйвер MPU6050 настраивает акселерометр/гироскоп
// на самую высокую чувствительность +/- 2g и +/- 250 град/сек

#define ACCEL_RAW_TO_FS(x) ((x)*2.0) / 32768.0
#define GYRO_RAW_TO_FS(x) ((x)*250.0) / 32768.0
#define TEMP_RAW_TO_FS(x) ((x) / 340.0) + 36.53

class Imu
{
public:
  MPU6050 mpu;
  Madgwick filter;
  unsigned long period, timer;
  float ax, ay, az, gx, gy, gz, t;
  float roll, pitch, yaw;

  bool init(unsigned long _period)
  {
    Wire.begin();
    mpu.initialize();

    // у MPU6050 частота обновления акселерометра - 1 КГц, гироскопа - 8 КГц,
    // поэтому возможность работать c периодом меньше 1 мс не нужна

    filter.begin(1000 / _period);
    period = _period;
    timer = 0;

    return mpu.testConnection();
  }

  bool getMotion()
  {
    int16_t iax, iay, iaz, igx, igy, igz, it;
    unsigned long now = millis();

    // фильтр настроен на обновление с периодом period, делаем так, чтобы чтение данных
    // выполнялось с этим же периодом

    if (now < timer)
      return false;

    timer += ((now - timer) / period + 1) * period;

    // читаем сырые данные сенсоров, диапазон значений +/- 32767
    mpu.getMotion6(&iax, &iay, &iaz, &igx, &igy, &igz);
    it = mpu.getTemperature();

    // переводим значения сенсоров в их единицы с учетом настроек чувствительности
    ax = ACCEL_RAW_TO_FS(iax);
    ay = ACCEL_RAW_TO_FS(iay);
    az = ACCEL_RAW_TO_FS(iaz);

    gx = GYRO_RAW_TO_FS(igx);
    gy = GYRO_RAW_TO_FS(igy);
    gz = GYRO_RAW_TO_FS(igz);

    t = TEMP_RAW_TO_FS(it);

    // обновляем фильтр и вычисляем ориентацию
    filter.updateIMU(gx, gy, gz, ax, ay, az);

    // roll: крен (крылья влево/вправо), вокруг оси X
    // pitch: тангаж (нос вверх/вниз), вокруг оси Y
    // yaw: рыскание (нос влево/вправо), вокруг оси Z

    roll = filter.getRoll();
    pitch = filter.getPitch();
    yaw = filter.getYaw();

    return true;
  }

  uint8_t getDeviceID()
  {
    return mpu.getDeviceID();
  }
};

#endif
