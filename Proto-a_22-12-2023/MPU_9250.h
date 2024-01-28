// Gestion MPU sans interruption
//--------------------------------

#include <MPU9250.h>

MPU9250 mpu;

// for calibrating the IMU module
//#define ACCEL_GYRO_CALIBRATION_MODE
//#define MAGNETOMETER_CALIBRATION_MODE

float yprAngle[3];
float gx, gy, gz;

//Drift Check
boolean stable = 0;
float stableValue = 99;

//#####################################################
boolean gyroIsStable()
{
  return stable;
}

//#####################################################
void gyroSetup()
{
  /////////////////////
  // MPU CONFIGURATION
  /////////////////////
  MPU9250Setting setting;
  setting.accel_fs_sel     = ACCEL_FS_SEL::A4G;
  setting.gyro_fs_sel      = GYRO_FS_SEL::G500DPS;
  setting.mag_output_bits  = MAG_OUTPUT_BITS::M16BITS;
  setting.fifo_sample_rate = FIFO_SAMPLE_RATE::SMPL_500HZ;
  setting.gyro_fchoice     = 0x03;
  setting.gyro_dlpf_cfg    = GYRO_DLPF_CFG::DLPF_41HZ;
  setting.accel_fchoice    = 0x01;
  setting.accel_dlpf_cfg   = ACCEL_DLPF_CFG::DLPF_45HZ;

  if (!mpu.setup(0x68) )
  {
    while (1) {
      Serial.println("IMU connection ratée!");
      delay(500);
    }
  } else Serial.println("IMU connection OK!");

#ifdef ACCEL_GYRO_CALIBRATION_MODE
#ifdef DEBUG
  Serial.println("Calibration  de l'accelerometre et du gyroscope...");
  Serial.println("Veuillez poser l'IMU sur une surface immobile!");
#endif
  for (int i = 5; i > 0; --i) {
#ifdef DEBUG
    Serial.printf("Calibration Accel&Gyro dans %d secondes\n", i);
#endif
    delay(1000);
  }
#ifdef DEBUG
  Serial.println("C'est parti !!");
#endif
  mpu.calibrateAccelGyro();
#ifdef DEBUG
  Serial.println("Calibration Accel&Gyro OK!");
#endif
#endif

#ifdef MAGNETOMETER_CALIBRATION_MODE
  mpu.setFilterIterations(20);
  mpu.setMagneticDeclination(+1.63f); // Dec 2023, Montpellier
#ifdef DEBUG
  Serial.println("Calibration du magnetomètre...");
  Serial.println("Veuillez faire pivoter l'IMU lorsque indiqué");
#endif
  for (int i = 5; i > 0; --i) {
#ifdef DEBUG
    Serial.printf("Calibration magnetomètre dans %d secondes\n", i);
#endif
    delay(1000);
  }
#ifdef DEBUG
  Serial.println("C'est parti !!");
#endif
  mpu.calibrateMag();
#ifdef DEBUG
  Serial.println("Calibration magnetomètre OK!");
#endif
#endif
}

//#####################################################
void gyroCheckStable()
{
  if (!stable) {
//    float checkSum = (ypr[0] + ypr[1] + ypr[2]) * 180 / M_PI;
    float checkSum = (yprAngle[0] + yprAngle[1] + yprAngle[2]);
    if (abs(checkSum - stableValue) < 1) stable = 1;
    stableValue = checkSum;
  }
}

//#####################################################
void MPULoop()
{
  if(mpu.update())
  {
  // Attention à l'ordre => yaw, pitch, roll
  yprAngle[0] = mpu.getYaw(); 
  yprAngle[1] = mpu.getPitch();
  yprAngle[2] = mpu.getRoll();

  gx = mpu.getGyroX();
  gy = mpu.getGyroY();
  gz = mpu.getGyroZ();
  }
}

//#####################################################
