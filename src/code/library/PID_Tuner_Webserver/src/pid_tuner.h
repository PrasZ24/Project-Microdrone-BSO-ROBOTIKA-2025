#ifndef pid_tuner_h
#define pid_tuner_h

#include "Arduino.h"
#include <AsyncTCP.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>

class pid_tuner
{
private:
    float yaw_p, yaw_i, yaw_d = 0;
    float pitch_p, pitch_i, pitch_d = 0;
    float roll_p, roll_i, roll_d = 0;

    float yaw_p_inner, yaw_i_inner, yaw_d_inner = 0;
    float pitch_p_inner, pitch_i_inner, pitch_d_inner = 0;
    float roll_p_inner, roll_i_inner, roll_d_inner = 0;

    int throttle, motorState = 0;

    float trim_m1 = 0.0;
    float trim_m2 = 0.0;
    float trim_m3 = 0.0;
    float trim_m4 = 0.0;

    AsyncWebServer server;
    AsyncEventSource events;

public:
    pid_tuner();
    void begin();
    void sendOutYaw_p(float value);
    void sendOutYaw_i(float value);
    void sendOutYaw_d(float value);

    void sendOutRoll_p(float value);
    void sendOutRoll_i(float value);
    void sendOutRoll_d(float value);

    void sendOutPitch_p(float value);
    void sendOutPitch_i(float value);
    void sendOutPitch_d(float value);

    void sendOutYawInner_p(float value);
    void sendOutYawInner_i(float value);
    void sendOutYawInner_d(float value);

    void sendOutRollInner_p(float value);
    void sendOutRollInner_i(float value);
    void sendOutRollInner_d(float value);

    void sendOutPitchInner_p(float value);
    void sendOutPitchInner_i(float value);
    void sendOutPitchInner_d(float value);

    void sendAngle_yaw(float value);
    void sendAngle_roll(float value);
    void sendAngle_pitch(float value);

    void sendGyro_yaw(float value);
    void sendGyro_roll(float value);
    void sendGyro_pitch(float value);

    void sendPWM_1(float value);
    void sendPWM_2(float value);
    void sendPWM_3(float value);
    void sendPWM_4(float value);

    float getYawP();
    float getYawI();
    float getYawD();

    float getPitchP();
    float getPitchI();
    float getPitchD();

    float getRollP();
    float getRollI();
    float getRollD();

    float getYawInnerP();
    float getYawInnerI();
    float getYawInnerD();

    float getPitchInnerP();
    float getPitchInnerI();
    float getPitchInnerD();

    float getRollInnerP();
    float getRollInnerI();
    float getRollInnerD();

    int getThrottle();
    int getMotorState();

    float getTrimM1();
    float getTrimM2();
    float getTrimM3();
    float getTrimM4();
};

#endif