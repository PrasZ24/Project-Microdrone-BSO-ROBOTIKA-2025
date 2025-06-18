#include <puc_v2.h>
#include <MPU6050_light.h>
#include "ArduPID.h"
#include <Preferences.h>

ArduPID pitch_pid;
ArduPID roll_pid;
MPU6050 sensor_mpu(Wire);
puc myController;
Preferences pref;

double pwm_0, pwm_1, pwm_2, pwm_3 = 0;
double angle_x, angle_y, angle_x_filtered, angle_y_filtered = 0;
double pitch_out, roll_out, set_point = 0;
double dataLY, dataLX, dataRY = 0;

void setup() {
  Wire.begin(7, 6);

  Serial.begin(9600);
  sensor_mpu.begin(1, 3);
  sensor_mpu.upsideDownMounting = false;
  sensor_mpu.setFilterGyroCoef(0.995);
  Wire.beginTransmission(0x68);
  Wire.write(0x1A);
  Wire.write(0x05);
  Wire.endTransmission();
  myController.begin("ESP32_WebServer", "123456789", IPAddress(192, 168, 4, 1));
  pref.begin("kalibrasiMPU", true);
  float ax = pref.getFloat("ax");
  float ay = pref.getFloat("ay");
  float az = pref.getFloat("az");
  float gx = pref.getFloat("gx");
  float gy = pref.getFloat("gy");
  float gz = pref.getFloat("gz");
  pref.end();
  sensor_mpu.setAccOffsets(ax, ay, az);
  sensor_mpu.setGyroOffsets(gx, gy, gz);
  Serial.println("MPU READY");

  pitch_pid.begin(&angle_x, &pitch_out, &dataLX, 1, 0, 0.2);
  pitch_pid.setOutputLimits(-127.5, 127.5);
  pitch_pid.setWindUpLimits(-10, 10);

  roll_pid.begin(&angle_y, &roll_out, &dataLY, 1, 0, 0.15);
  roll_pid.setOutputLimits(-127.5, 127.5);
  roll_pid.setWindUpLimits(-16, 16);

  ledcSetup(0, 1500, 8);
  ledcSetup(1, 1500, 8);
  ledcSetup(2, 1500, 8);
  ledcSetup(3, 1500, 8);
  ledcAttachPin(1, 0);
  ledcAttachPin(2, 1);
  ledcAttachPin(3, 2);
  ledcAttachPin(4, 3);
}

void loop() {
  myController.DNS_RUN();
  sensor_mpu.update();
  if (myController.device_status()) {
    dataRY = map(myController.getRX(), -128, 127, 0, 200);
    dataLY = map(myController.getLY(), 0, 127, 0, 25);
    dataLX = map(myController.getLX(), 0, 127, 0, 25);
    angle_x = round(-sensor_mpu.getAngleX());
    angle_y = round(-sensor_mpu.getAngleY());

    pitch_pid.compute();
    roll_pid.compute();

    pwm_0 = dataRY + pitch_out + roll_out;  // pwm_0
    pwm_1 = dataRY + pitch_out - roll_out;  // pwm_1
    pwm_2 = dataRY - pitch_out + roll_out;  // pwm_2
    pwm_3 = dataRY - pitch_out - roll_out;

    pwm_0 = constrain(pwm_0, 0, 255);
    pwm_1 = constrain(pwm_1, 0, 255);
    pwm_2 = constrain(pwm_2, 0, 255);
    pwm_3 = constrain(pwm_3, 0, 255);

    // ledcWrite(0, pwm_0);
    // ledcWrite(1, pwm_1);
    // ledcWrite(2, pwm_2);
    // ledcWrite(3, pwm_3);

    // Cetak data PID dan PWM
    // Serial.print("AngleX: ");
    Serial.print(100);
    Serial.print(",");
    Serial.print(angle_y, 2);
    Serial.print(",");
    Serial.print(-100);
    Serial.print(",");
    Serial.println(angle_x, 2);

  } else {
    dataLX = 0;
    dataLY = 0;
    dataRY = 0;
    angle_y = 0;
    angle_x = 0;
    pitch_out = 0;
    roll_out = 0;

    pwm_0 = 0;
    pwm_1 = 0;
    pwm_2 = 0;
    pwm_3 = 0;

    ledcWrite(0, 0);
    ledcWrite(1, 0);
    ledcWrite(2, 0);
    ledcWrite(3, 0);
  }
}