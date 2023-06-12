#include <Wire.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include "Gyro.h"

GyroClass Gyro;
MPU6050 Mpu;

bool GyroClass::init()
{
  uint8_t devStatus;

  deviceID = 0;
  dmpReady = false;

  // join I2C bus (I2Cdev library doesn't do this automatically)
  Wire.begin();
  Wire.setClock(400000); // 400kHz I2C clock

  // initialize MPU6050
  Mpu.initialize();

  // verify connection
  Serial.println("Testing MPU6050 connection...");
  if (!Mpu.testConnection())
  {
    Serial.println("MPU6050 connection failed");
    return false;
  }

  Serial.println("MPU6050 connection successful");
  deviceID = Mpu.getDeviceID();

  // load and configure the DMP
  Serial.println("Initializing DMP...");
  devStatus = Mpu.dmpInitialize();
  if (devStatus != 0)
  {
    Serial.print("DMP initialization failed (code ");
    Serial.print(devStatus);
    Serial.println(")");
    return false;
  }
  Serial.println();

  // Calibration Time: generate offsets and calibrate our MPU6050
  Mpu.CalibrateAccel(6);
  Mpu.CalibrateGyro(6);

  // turn on the DMP, now that it's ready
  Serial.println("Enabling DMP...");
  Mpu.setDMPEnabled(true);

  // set our DMP Ready flag so the getMotion() function knows it's okay to use it
  Serial.println("DMP is ready");
  dmpReady = true;

  return true;
}

bool GyroClass::getMotion()
{
  uint8_t fifoBuffer[64]; // FIFO storage buffer
  Quaternion q;           // [w, x, y, z]         quaternion container
  VectorFloat gravity;    // [x, y, z]            gravity vector
  float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

  // if programming failed, don't try to do anything
  if (!dmpReady)
    return false;

  // read a packet from FIFO
  if (!Mpu.dmpGetCurrentFIFOPacket(fifoBuffer))
    return false;

  // get yaw/pitch/roll angles
  Mpu.dmpGetQuaternion(&q, fifoBuffer);
  Mpu.dmpGetGravity(&gravity, &q);
  Mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

  // roll: крен (крыло вверх/вниз), вокруг оси X
  // pitch: тангаж (нос вверх/вниз), вокруг оси Y
  // yaw: рыскание (нос влево/вправо), вокруг оси Z
  
  roll = ypr[2] * 180 / M_PI;
  pitch = ypr[1] * 180 / M_PI;
  yaw = ypr[0] * 180 / M_PI;

  return true;
}
