#include <pid_tuner.h>

pid_tuner tuner;

float kpYaw, kiYaw, kdYaw = 0;
float kpPitch, kiPitch, kdPitch = 0;
float kpRoll, kiRoll, kdRoll = 0;

float yaw_p = 0;
void setup() {
  Serial.begin(9600);
  tuner.begin();
}

void loop() {
  //reading only yaw axis
  kpYaw = tuner.getYawP();
  kiYaw = tuner.getYawI();
  kdYaw = tuner.getYawD();
  Serial.print("Kp Yaw : ");
  Serial.print(kpYaw, 3);
  Serial.print(" Ki Yaw : ");
  Serial.print(kiYaw, 3);
  Serial.print(" Kd Yaw : ");
  Serial.println(kdYaw, 3);

  //send data :
  tuner.sendOutYaw_p(yaw_p);
}