#include "I2Cdev.h"
#include "MPU6050.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif
#define OUTPUT_READABLE_ACCELGYRO
#define flexSensorPin A1
#define flexSensorPin1 A2

//Variaveis

MPU6050 accelgyro;
int16_t ax, ay, az;
//int16_t gx, gy, gz;
//valores a serem enviados pelo arduino
int x, y, z, auxz;
int minx = 9000, maxx = 11200, miny = -4000, maxy = -2000, minz = 2400, maxz = 3700;
int ruimS, bomS, ruimMap, bomMap;
//int g2 = 16384, g4 = 8192, g8 = 4096, g16 = 2048;

//sda a4, scl a5

void setup() {
  // join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif
  Serial.begin(112500);
  accelgyro.initialize();
  accelgyro.setFullScaleAccelRange(MPU6050_ACCEL_FS_8);
  accelgyro.setFullScaleGyroRange(MPU6050_GYRO_FS_1000);
  accelgyro.setXAccelOffset(-76);
  accelgyro.setYAccelOffset(-2359);
  accelgyro.setZAccelOffset(1688);
  //  accelgyro.setXGyroOffset(220);
  //  accelgyro.setYGyroOffset(76);
  //  accelgyro.setZGyroOffset(-85);

}
void(* resetFunc)(void) = 0;
void loop() {

  //accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  ruimS = analogRead(A1);
  bomS = analogRead(A2);
  ruimMap = map(ruimS, 4, 0, 0, 100);
  bomMap = map(bomS,  64, 198, 100, 0);
  if (ruimMap < 0) {
    ruimMap = 0;
  }
  if (ruimMap > 100) {
    ruimMap = 100;
  }
  if (bomMap < 0) {
    bomMap = 0;
  }
  if (bomMap > 100) {
    bomMap = 100;
  }
  accelgyro.getAcceleration(&ax, &ay, &az);
  //  accelgyro.getRotation(&gx, &gy, &gz);
#ifdef OUTPUT_READABLE_ACCELGYRO
  //conversao do valor x para eixo X na unity
  if (ax >= minx && ax <= maxx) x = 0;
  if (ax < minx) x = 1;
  if (ax > maxx) x = -1;
  //conversao do valor y para eixo Z na unity
  if (ay >= miny && ay <= maxy) y = 0;
  if (ay < miny) y = 1;
  if (ay > maxy) y = -1;
  //conversao do eixo z para eixo Y na unity
  if (az >= minz && az <= maxz) {
    z = 0;
    auxz = 0;
  }
  if (az < minz && auxz == 0) {
    if (x == 0 || y == 0) {
      z = -1;
      auxz = -1;
    }
  }
  if (az > maxz && auxz == 0) {
    if (x == 0 || y == 0) {
      z = 1;
      auxz = 1;
    }
  }
  Serial.print(x);
  Serial.print(",");
  Serial.print(z);
  Serial.print(",");
  Serial.print(y);
  Serial.print(",");
  Serial.print(ruimMap);
  Serial.print(",");
  Serial.print(bomMap);
  Serial.println("");
  delay(200);
#endif
  if ( ax == 0 && ay == 0 && az == 0) {
    delay(10);
    resetFunc();
  }
}

